.macro do_nothing
  nop
  nop
.endm

  .text
  .global _start

_start:
  lui a4, 0x80000
  li  x5, 0x11111111
  sw  x5, 112(a4)
  li  x6, 0xffffffff
  sw  x6, 116(a4)
  lw  x8, 112(a4)
  lw  x9, 116(a4)
  do_nothing

stop: j stop

  .end
