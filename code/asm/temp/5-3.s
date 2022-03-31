.macro do_nothing
  nop
  nop
.endm

  .text
  .global _start

_start:
  li x5, 1
  li x6, 2
  li x7, 3
  li x8, 4
  add x5, x5, x6
  add x7, x7, x8
  sub x8, x5, x7
  do_nothing

stop: j stop
  
  .end
