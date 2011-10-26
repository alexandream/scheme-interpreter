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
;;; Fixnum relational operators.
;;;
(define (> a1 a2)
  (< a2 a1))

(define (>= a1 a2)
    (not (< a1 a2)))

(define (<= a1 a2)
    (not (< a2 a1)))

(define (= a1 a2)
    (if (not (< a1 a2))
      (not (< a2 a1))
      #F))


;;;
;;; Auxiliary list operations.
;;;
(define (list? l)
    (if (pair? l)
      (list? (cdr l))
      (if (null? l)
        #T
        #F)))

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

(define *current-exception-handlers* (make-parameter (list #F)))

(define (with-exception-handler handler thunk)
    (with-exception-handlers (cons handler (*current-exception-handlers*))
                             thunk))

(define (with-exception-handlers handler-list thunk)
  (parameterize ((*current-exception-handlers* handler-list))
    (thunk)))


    

