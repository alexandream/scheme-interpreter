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

(define reverse
  (lambda (l)
    (if (null? l)
      '()
      (append (reverse (cdr l)) (cons (car l) '()) ))))

