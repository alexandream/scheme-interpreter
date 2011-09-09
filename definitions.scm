(define map (lambda (func list)
              (if (eq? '() list)
                '()
                (cons (func (car list))
                      (map func (cdr list))))))


(define append 
  (lambda (l1 l2)
    (if (eq? '() l1)
      l2
      (cons (car l1) (append (cdr l1) l2)))))


(define reverse
  (lambda (l)
    (if (eq? '() l)
      '()
      (append (reverse (cdr l)) (cons (car l) '()) ))))

