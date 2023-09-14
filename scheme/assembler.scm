(import (chibi) (scheme load) (scheme base) (srfi 125) (scheme process-context))

;;; Define r0 to r1023
(let loop ((i 0))
  (if (< i 1024)
      (let ((reg-name (string->symbol (string-append "r" (number->string i)))))
        (eval `(define ,reg-name ,i) (interaction-environment))
        (loop (+ i 1)))))  

(define *restart-assembler-continuation* #f)
(define *pc* 65536)
(define *previous-symbol-table* '())
(define *symbol-table* (make-hash-table eq?))

;;; (define *output-port* #f)
(define *output-bv* #f)
(define *output-number-of-bytes* 0)

(define (assembler:start)
  (display "Starting assembler...\n" (current-error-port))
  (call-with-current-continuation
   (lambda (restart)
     (set! *restart-assembler-continuation* restart)))
  (set! *output-bv* (make-bytevector 1024))
  (set! *output-number-of-bytes* 0)
  (set! *pc* 65536)
  (set! *previous-symbol-table* *symbol-table*)
  (set! *symbol-table* (hash-table-copy *symbol-table*)))
  
(define (assembler:end)
  (if (symbol-table-dirty?)
      (begin
        (display "(restarting assembler...\n" (current-error-port))
        (*restart-assembler-continuation* #f))
      (let ((bytes (get-output-bytes)))
        (display "Finishing assembly..\n" (current-error-port))
        (write-bytevector bytes)
        (exit #t))))

(define (get-output-bytes)
  (let ((trimmed (make-bytevector *output-number-of-bytes*)))
    (bytevector-copy! trimmed 0 *output-bv*)
    ;; (write trimmed)
    ;; (display "\n")
    trimmed))

;;; Symbol Tables

(define (label name)
  (hash-table-set! *symbol-table* name *pc*))

(define (address name)
  (hash-table-ref/default *symbol-table* name 0))

(define (symbol-table-dirty?)
  (not (equal? *symbol-table* *previous-symbol-table*)))
  
(define (assembler:output-byte! b)
  ;; (display "assembler:output-byte!")
  ;; (display b)
  ;; (display "\n")
  (let ((len (bytevector-length *output-bv*)))
    (if (< *output-number-of-bytes* len)
        (begin
          (bytevector-u8-set! *output-bv* *output-number-of-bytes* b)
          (set! *pc* (+ *pc* 1))
          (set! *output-number-of-bytes* (+ *output-number-of-bytes* 1)))
        (let ((bv (make-bytevector (* 2 (bytevector-length *output-bv*)))))
          (bytevector-copy! *output-bv* 0 bv len)
          (assembler:output-byte! b))))
  ;; (write *output-bv*)
  ;; (display "\n")
  )

;;; implement me!
(define (assembler:output-uleb128! number)
  (assembler:output-byte! number))

(define *opcode-mov* 13)

(define (mov dst-reg src-reg)
  (assembler:output-uleb128! *opcode-mov*)
  (assembler:output-uleb128! dst-reg)
  (assembler:output-uleb128! src-reg))

(define (add dst-reg src1-reg src2-reg)
  (assembler:output-uleb128! *opcode-mov*)
  (assembler:output-uleb128! dst-reg)
  (assembler:output-uleb128! src1-reg)
  (assembler:output-uleb128! src2-reg))
