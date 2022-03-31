.macro set_struct
  sw x7, 112(a4)
  sw x8, 116(a4)
.endm

.macro get_struct
  lw x7, 112(a4)
  lw x8, 116(a4)
.endm

  .text
  .global _start

_start:
  lui a4, 0x80000
  li x5, 0
  sw x5, 112(a4)
  li x6, 0
  sw x6, 116(a4)

foo:
  li x7, 0x12345678
  sw x7, 120(a4)
  li x8, 0x87654321
  sw x8, 124(a4)
  set_struct 
  li x7, 0
  li x8, 0
  get_struct

stop: j stop
  
  .end
