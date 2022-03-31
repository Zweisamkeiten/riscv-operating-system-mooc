.macro do_nothing
  nop
  nop
.endm

  .text
  .global _start

_start:
  li  x5, 1
  li  x6, 2
  li  x7, 3
  add x8, x5, x6
  sub x9, x8, x7
  do_nothing

stop: j stop

  .end
