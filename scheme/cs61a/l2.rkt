#lang racket

;; programing paradimgs
;;
;;
;;
;;
;;
;;
;;

(define (argue s)
  (if (empty? s)
      `()
      (cons (opposite (car s))
                (argue (cdr s)))))
(define (opposite w)
  (cond ((equal? w `like) `hate)
        ((equal? w `hate) `like)
        ((equal? w `wonderful) `terrible)
        ((equal? w `terrible) `wonderful)
        (else w)))

;; (define (~ s)
;;   (if (empty? s)
;;       `()
;;       (cons (_ (car s))
;;             (~ (cdr s)))))
