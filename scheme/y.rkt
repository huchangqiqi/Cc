(define length
  (lambda (ls)
    (cond
      [(null? ls) 0]
      [else (add1 (length (cdr ls)))])))

((lambda (length)
  (lambda (ls)
    (cond
      [(null? ls) 0]
      [else (add1 ((length length)(cdr ls)))])))
(lambda (length)
  (lambda (ls)
    (cond
      [(null? ls) 0]
      [else (add1 ((length length)(cdr ls)))]))))

;; outer
((lambda (u)(u u))
 (lambda (length)
   (lambda (ls)
     (cond
       [(null? ls) 0]
       [else (add1 ((length length)(cdr ls)))]))))

