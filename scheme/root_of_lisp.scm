(define append. (x y)
  (cond ((null. x) y)
        (else (cons (car x)(append. (cdr x) y)))))
