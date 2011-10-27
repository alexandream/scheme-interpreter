($DEFINE null? (lambda (x) (eq? '() x)))

($DEFINE append 
  (lambda (l1 l2)
    (if (null? l1)
      l2
      (cons (car l1) (append (cdr l1) l2)))))
;; Yeap... without the binding forms, using only lambda, this shit is ugly
;; as hell. Too bad we don't have them yet, right? :)
(define-rewriter quasiquote
  (lambda (expr)
   (((lambda (unquote-splicing? unquote?)   
       ((lambda (expand-quasiquote)
          (set! expand-quasiquote 
            (lambda (expr)
              (if (not (pair? expr))
                (list 'quote expr)
                (if (unquote? expr)
                  (car (cdr expr))
                  (if (unquote-splicing? (car expr))
                    (list 'append (car (cdr (car expr))) 
                          (expand-quasiquote (cdr expr)))
                    (list 'cons 
                          (expand-quasiquote (car expr)) 
                          (expand-quasiquote (cdr expr))))))))
          expand-quasiquote) #F))
     ;; unquote-splicing?
     (lambda (expr)
       (if (pair? expr)
         (eq? 'unquote-splicing (car expr))
         #F))
     ;; unquote?
     (lambda (expr)
       (if (pair? expr)
         (eq? 'unquote (car expr))
         #F)))
    (car expr))))

(define-rewriter define
  (lambda (args)
    (if (symbol? (car args))
      `($DEFINE ,@args)
      `($DEFINE ,(car (car args))
                (lambda ,(cdr (car args)) ,@(cdr args))))))

(define-rewriter and
  (lambda (args)
    (if (null? args)
      #T
      `(if ,(car args)
         (and ,@(cdr args))
         #F))))

(define-rewriter or
  (lambda (args)
    (if (null? args)
      #F
      `(if ,(car args)
         #T
         (or ,@(cdr args))))))

(define-rewriter begin
  (lambda (args)
    `((lambda () ,@args))))

(define (map func list)
              (if (null? list)
                '()
                (cons (func (car list))
                      (map func (cdr list)))))

;; Finally we'll have the binding forms. This is still ugly, but it's the 
;; last time. At least this time we have quasiquotation. :)
(define-rewriter let
   (lambda (args)
     ((lambda (first rest)
        (if (symbol? first)
          ((lambda (binds body)
             `(letrec ((,first (lambda ,(map car binds) ,@body)))
                (,first ,@(map (lambda (x) (car (cdr x))) binds))))
           (car rest) (cdr rest))
          ((lambda (binds body) 
             `((lambda ,(map car binds) ,@body)
               ,@(map (lambda (y) (car (cdr y))) binds)))
           first rest)))
      (car args) (cdr args))))

(define-rewriter let* 
   (lambda (args)
     (let ((bindings (car args))
           (body (cdr args)))
       (if (null? bindings)
         `(let () ,@body)
         (let ((first-bind (car bindings)))
           `(let ((,(car first-bind) ,(car (cdr first-bind))))
              (let* ,(cdr bindings) ,@body)))))))

(define-rewriter letrec  
   (lambda (args)
     (let ((binds (car args))
           (body (cdr args)))
       (let ((newbinds (map (lambda (z) (list (car z) #F)) binds))
             (setters (map (lambda (w) (list 'set!  (car w) (car (cdr w))))
                           binds)))
         `(let ,newbinds ,@setters ,@body)))))

(define-rewriter delay
  (lambda (args)
    (let ((memo (gensym))
          (expression (car args)))
      `(let ((,memo #U))
         (lambda ()
           (if (unspecified? ,memo)
             (set! ,memo ((lambda () ,expression))))
           ,memo)))))

(define (force promise)
    (promise))


;;;
;;; Auxiliary list operations.
;;;
(define (list? l)
    (if (pair? l)
      (list? (cdr l))
      (if (null? l)
        #T
        #F)))

(define (make-list n . xs)
  (letrec ((make-list-aux (lambda (n elem)
                            (if (= n 0)
                              '()
                              (cons elem (make-list-aux (- n 1) elem))))))
    (make-list-aux n (if (pair? xs) (car xs) #U))))


(define (length l)
    (letrec ((aux-length (lambda (lst n)
                           (if (null? lst)
                             n
                             (aux-length (cdr lst) (+ n 1))))))
      (aux-length l 0)))

(define (reverse l)
    (if (null? l)
      '()
      (append (reverse (cdr l)) (cons (car l) '()) )))

(define (filter f l)
    (if (null? l)
      '()
      (let ((head (car l))
            (tail (cdr l)))
        (if (f head)
          (cons head (filter f tail))
          (filter f tail)))))

(define (list-tail x k)
    (if (= k 0)
      x
      (list-tail (cdr x) (- k 1))))


(define (list-ref x k)
    (car (list-tail x k)))

(define (list-set! lst n val)
  (set-car! (list-tail lst n) val))

(define (member obj lst . rest)
  (let ((compare (if (pair? rest) 
                   (car rest)
                   equal?)))
    (letrec ((member-aux (lambda (obj lst cmp)
                           (if (null? lst)
                             #F
                             (if (cmp (car lst) obj)
                               lst
                               (member-aux obj (cdr lst) cmp))))))
      (member-aux obj lst compare))))

(define (memq obj lst)
  (member obj lst eq?))

(define (memv obj lst)
  (member obj lst eqv?))

(define (assoc obj alist . rest)
  (let ((compare (if (pair? rest)
                   (car rest)
                   equal?)))
    (letrec ((assoc-aux (lambda (obj alist cmp)
                          (if (null? alist)
                            #F
                            (if (cmp (car (car alist)) obj)
                              (car alist)
                              (assoc-aux obj (cdr alist) cmp))))))
      (assoc-aux obj alist compare))))

(define (assq obj alist)
  (assoc obj alist eq?))

(define (assv obj alist)
  (assoc obj alist eqv?))

(define (list-copy lst)
  (if (null? lst)
    '()
    (cons (car lst) (list-copy (cdr lst)))))

;;;
;;; Numeric functions.
;;;
(define (identity x) x)
(define number? integer?)
(define rational? integer?)
(define complex? integer?)
(define real? integer?)
(define exact? integer?)
(define (inexact? x) #F)
(define exact-integer? integer?)
(define finite? integer?)
(define (infinite? x) #F)
(define (nan? x) #F)

(define (zero? x) (= x 0))
(define (positive? x) (> x 0))
(define (negative? x) (< x 0))

(define numerator identity)
(define (denominator x) 1)
(define floor identity)
(define ceiling identity)
(define truncate identity)
(define round identity)
(define (rationalize x y) x)   

(define max #U)
(define min #U)
(let ((aux-min-max (lambda (x xs f)
                     (let loop ((x x) (xs xs))
                       (if (null? xs)
                         x
                         (let ((y (car xs)))
                           (loop (if (f x y) 
                                   x 
                                   y)
                                 (cdr xs))))))))
  (set! max (lambda (x . xs)
              (aux-min-max x xs >)))
  (set! min (lambda (x . xs)
              (aux-min-max x xs <))))

(define (even? x)
  (zero? (modulo x 2)))

(define (odd? x)
  (not (even? x)))


(define real-part identity)
(define (imag-part x) 0)
(define magnitute identity)
(define (angle x) 0)

;;;
;;; Dynamic wind.
;;;
(define dynamic-wind #F)
(define call-with-current-continuation #F)
(define call/cc #F)
(let ((winders '()))
  (letrec ((common-tail (lambda (x y)
                          (let ((lx (length x)) (ly (length y)))
                            (let loop ((x (if (> lx ly) (list-tail x (- lx ly)) x))
                                       (y (if (> ly lx) (list-tail y (- lx ly)) y)))
                              (if (eq? x y)
                                x
                                (loop (cdr x) (cdr y)))))))
           (do-wind (lambda (new)
                      (let ((tail (common-tail new winders)))
                        (let f ((l winders))
                          (if (not (eq? l tail))
                            (begin
                              (set! winders (cdr l))
                              ((cdr (car l)))
                              (f (cdr l)))))
                        (let f ((l new))
                          (if (not (eq? l tail))
                            (begin
                              (f (cdr l))
                              ((car (car l)))
                              (set! winders l))))))))
  (set! call/cc
    (lambda (f)
      ($CALL-CC (lambda (k)
           (f (let ((save winders))
                (lambda (x)
                  (if (not (eq? save winders)) (do-wind save))
                  (k x))))))))

  (set! call-with-current-continuation call/cc)
  (set! dynamic-wind
    (lambda (in body out)
      (in)
      (set! winders (cons (cons in out) winders))
      (let ((ans (body)))
        (set! winders (cdr winders))
        (out)
        ans)))))

;;;
;;; Parameters
;;;
(define (make-parameter value . rest)
  (let* ((converter (if (null? rest)
                      (lambda (x) x)
                      (car rest)))
         (value     (converter value)))
    (lambda args
      (if (null? args)
        value
        (set! value (converter (car args)))))))

(define-rewriter parameterize 
  (lambda (args)
    (let ((info (map (lambda (x) (list (car x)
                                     (gensym)
                                     (car (cdr x))))
                   (car args)))
        (body (cdr args)))
    (let ((binds  (map (lambda (x) `(,(car (cdr x)) (,(car x)))) info))
          (sets   (map (lambda (x) `(,(car x) ,(car (cdr (cdr x))))) info))
          (resets (map (lambda (x) `(,(car x) ,(car (cdr x)))) info)))
      `(let ,binds
         (dynamic-wind
           (lambda () ,@sets)
           (lambda () ,@body)
           (lambda () ,@resets)))))))

;;;
;;; Exception Handling System
;;;
(define *current-exception-handlers* (make-parameter (list #F)))

(define (with-exception-handler handler thunk)
    (with-exception-handlers (cons handler (*current-exception-handlers*))
                             thunk))

(define (with-exception-handlers handler-list thunk)
  (parameterize ((*current-exception-handlers* handler-list))
    (thunk)))

(define (error message . objs)
  ;; TODO: Implement error.
  #U)

(define (raise obj)
  (let ((handlers (*current-exception-handlers*)))
    (with-exception-handlers (cdr handlers)
      (lambda ()
        ((car handlers) obj)
        (error "Exception Handler should not return, but it did"
               (car handlers)
               obj)))))


        


    

