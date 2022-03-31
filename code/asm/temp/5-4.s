.macro do_nothing
  nop
  nop
.endm

  .text
  .global _start

_start:
  li x5, 0x87654321
  li x6, 0xffff
  and x8, x5, x6
  srli x7, x5, 16
  and x9, x7, x6
  do_nothing

stop: j stop

  .end

