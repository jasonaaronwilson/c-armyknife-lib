(label '_start)
(mov r0 r1)
;;; this is illegal but accepted. hmmm...
(mov r1 (address 'start))
(mov r2 (address 'forward-reference))
(mov r3 #xffff)
(mov r3 #xffff)
(mov r3 #xffff)
(label 'forward-reference)



