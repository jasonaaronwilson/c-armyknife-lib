(label "_start")
(mov r0 r1)
;;; this is illegal but accepted. hmmm...
(mov r1 (address "_start"))
(mov r2 (address "forward-reference"))


