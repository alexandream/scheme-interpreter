(define null? (lambda (x) (eq? '() x)))
;;;
;;; Fixnum relational operators.
;;;
(define >
  (lambda (a1 a2)
    (< a2 a1)))

(define >=
  (lambda (a1 a2)
    (not (< a1 a2))))

(define <=
  (lambda (a1 a2)
    (not (< a2 a1))))

(define =
  (lambda (a1 a2)
    (if (not (< a1 a2))
      (not (< a2 a1))
      #F)))

(define append 
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



(define map (lambda (func list)
              (if (null? list)
                '()
                (cons (func (car list))
                      (map func (cdr list))))))

(define-rewriter begin
  (lambda (args)
    `((lambda () ,@args))))

(define-rewriter call/cc
  (lambda (args)
    (cons 'call-with-current-continuation args)))

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


(define-rewriter force
  (lambda (args)
    (list (car args))))



(define list?
  (lambda (l)
    (if (pair? l)
      (list? (cdr l))
      (if (null? l)
        #T
        #F))))

(define length
  (lambda (l)
    (letrec ((aux-length (lambda (lst n)
                           (if (null? lst)
                             n
                             (aux-length (cdr lst) (fixnum+ n 1))))))
      (aux-length l 0))))

(define reverse
  (lambda (l)
    (if (null? l)
      '()
      (append (reverse (cdr l)) (cons (car l) '()) ))))

(define filter
  (lambda (f l)
    (if (null? l)
      '()
      (let ((head (car l))
            (tail (cdr l)))
        (if (f head)
          (cons head (filter f tail))
          (filter f tail))))))
