;;;
;;; This is the outline of a simple assembler for the comet-vm.
;;;
;;; This works by adding to the environment useful global variables
;;; like r13 as well as functions for opcodes like "mov". Then we use
;;; "load" to "exectute" the statements in each assembly source file
;;; from the command line. If the symbol table has changed (because a
;;; symbol has moved), we loop back to a saved continuation and reload
;;; and re-execute.
;;;

(import (chibi)
        (scheme load)
        (scheme base)
        (srfi 125) ;; hash-tables
        (scheme process-context)
        (srfi 151)
        (srfi 95) ;; sort
        )

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
        (display "...restarting assembler...\n" (current-error-port))
        (*restart-assembler-continuation* #f))
      (let ((bytes (get-output-bytes)))
        (display "Finishing assembly..\n" (current-error-port))
        (write-bytevector bytes)
        (assembler:dump-symbol-table)
        (exit #t))))

(define (assembler:dump-symbol-table)
  (let* ((keys (sort (hash-table-keys *symbol-table*))))
    (for-each (lambda (key)
                (write key (current-error-port))
                (display ": " (current-error-port))
                (write (hash-table-ref/default *symbol-table* key 0)
                       (current-error-port))
                (display "\n" (current-error-port)))
              keys)))

(define (get-output-bytes)
  (let ((trimmed (make-bytevector *output-number-of-bytes*)))
    (bytevector-copy! trimmed 0 *output-bv*)
    trimmed))

;;; Symbol Tables

(define (label name)
  (hash-table-set! *symbol-table* name *pc*))

(define (address name)
  (hash-table-ref/default *symbol-table* name 0))

(define (symbol-table-dirty?)
  (not (equal? *symbol-table* *previous-symbol-table*)))
  
(define (assembler:output-byte! b)
  (let ((len (bytevector-length *output-bv*)))
    (if (< *output-number-of-bytes* len)
        (begin
          (bytevector-u8-set! *output-bv* *output-number-of-bytes* b)
          (set! *pc* (+ *pc* 1))
          (set! *output-number-of-bytes* (+ *output-number-of-bytes* 1)))
        (let ((bv (make-bytevector (* 2 (bytevector-length *output-bv*)))))
          (bytevector-copy! *output-bv* 0 bv len)
          (assembler:output-byte! b)))))

(define (assembler:output-uleb128! number)
  (if (< number 0)
      (error "assembler:output-uleb128! called with negative number"))
  ;; make sure number is positive!
  (let* ((b (bitwise-and number #x7f))
         (remainder (arithmetic-shift number -7)))
    (if (> remainder 0)
        (set! b (bitwise-ior b #x80)))
    (assembler:output-byte! b)
    (if (> remainder 0)
        (assembler:output-uleb128! remainder))))

(define *opcode-imm* 3)
(define *opcode-mov* 8)

(define (imm dst-reg immediate)
  (assembler:output-uleb128! *opcode-imm*)
  (assembler:output-uleb128! dst-reg)
  (assembler:output-uleb128! immediate))

(define (mov dst-reg src-reg)
  (assembler:output-uleb128! *opcode-mov*)
  (assembler:output-uleb128! dst-reg)
  (assembler:output-uleb128! src-reg))

(define (add dst-reg src1-reg src2-reg)
  (assembler:output-uleb128! *opcode-mov*)
  (assembler:output-uleb128! dst-reg)
  (assembler:output-uleb128! src1-reg)
  (assembler:output-uleb128! src2-reg))
