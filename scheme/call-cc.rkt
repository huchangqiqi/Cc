#lang racket

(define (generate-one-element-at-a-time lst)
  (define (control-state return)
    (for-each
     (lambda (element)
       (set! return (call-with-current-continuation
                     (lambda (resume-here)
                       ;;
                       (set! control-state resume-here)
                       (return element)))))
     lst)
    (return 'you-fell-off-the-end))
  (define (generator)
    (call-with-current-continuation control-state))
generator)

(define generate-digit
  (generate-one-element-at-a-time '(0 1 2)))

(generate-digit);; 0
(generate-digit);; 1
(generate-digit);; 2
(generate-digit);; you-fell-off-the-end


