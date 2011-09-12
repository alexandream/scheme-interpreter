(define null? (lambda (x) (eq? '() x)))

(define map (lambda (func list)
              (if (null? list)
                '()
                (cons (func (car list))
                      (map func (cdr list))))))


(define append 
  (lambda (l1 l2)
    (if (null? l1)
      l2
      (cons (car l1) (append (cdr l1) l2)))))

(define-rewriter begin
  (lambda (args)
    (list(append (list 'lambda '()) args))))


(define-rewriter let
  (lambda (args)
    ((lambda (first rest)
       (if (symbol? first)
         ((lambda (binds body)
            (list 'letrec 
                  (list (list first (append (list 'lambda (map car binds)) body)))
                  (append (list first) (map (lambda (x) (car (cdr x))) binds)))
 
            )
          (car rest) (cdr rest))
         ((lambda (binds body) 
            (append (list (append (list 'lambda (map car binds)) body))
                    (map (lambda (x) (car (cdr x))) binds)))
          first rest)))
     (car args) (cdr args))))
 

(define-rewriter letrec
  (lambda (args)
    (let ((binds (car args))
          (body (cdr args)))
      (let ((newbinds (map (lambda (x) (list (car x) #F)) binds))
            (setters (map (lambda (x) (list 'set!  (car x) (car (cdr x))))
                          binds)))
        (append (append (list 'let newbinds) setters) body)))))

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
