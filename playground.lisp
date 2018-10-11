;; Playground/testing stuff

(define some-string "this 'is a () test ; string")

;; Nested env and closures
(define make-account
    (lambda (balance)
      (lambda (amt)
        (set! balance (+ balance amt))
        balance)))
(define acc (make-account 100.0))
(print "Result: ")
(println (acc 20.0))

;; Recursive function
(define fact (lambda (n)
               (cond ((<= n 1) 1)
                     (t (* n (fact (- n 1)))))))
(fact 10)

