# Calling Convention

#void _start()
#{
#  sum_square(3);
#}
#
#unsigned int square(unsigned int i)
#{
# return i * i;
#}
#
#unsigned int sum_square(unsigned int i)
#{
# unsigned int sum = 0;
# for (int j = 1; j <= i; j++) {
#   sum += square(j);
# }
# return sum;
#}

  .text
  .global _start

_start:
  la sp, stack_end

# sum_squares(3);
  li a0, 3
  call sum_squares

stop:
  j stop

sum_squares:
  # prologue
  addi sp, sp, -16
  sw  s0, 0(sp)
  sw  s1, 4(sp)
  sw  s2, 8(sp)
  sw  ra, 12(sp)

  # cp and store the input params i
  mv  s0, a0

  # sum will be stored in s1 and is initialized as zero, j will be stored in s2
  li  s1, 0 # sum
  li  s2, 1 # j

  bge s0, s2, loop

  mv  a0, s1

  # epilogue
  lw  s0, 0(sp)
  lw  s1, 4(sp)
  lw  s2, 8(sp)
  lw  ra, 12(sp)
  addi sp, sp, 16
  ret

# for (int j = 1; j <= i; j++) {
loop:
  mv  a0, s2
  jal square
  add s1, s1, a0
  mv  a0, s0

  addi s2, s2, 1
  bge s0, s2, loop
  
  mv  a0, s1
  # epilogue
  lw  s0, 0(sp)
  lw  s1, 4(sp)
  lw  s2, 8(sp)
  lw  ra, 12(sp)
  addi sp, sp, 16
  ret

square:
  # prologue
  addi sp, sp, -8
  sw  s0, 0(sp)
  sw  s1, 4(sp)

  # `mul a0, a0, a0` should be fine
  mv  s0, a0
  mul s1, s0, s0
  mv  a0, s1

  # epilogue
  lw  s0, 0(sp)
  lw  s1, 4(sp)
  addi sp, sp, 8

  ret
  
# allocate stack space
stack_start:
	.rept 12
	.word 0
	.endr
stack_end:

	.end			# End of file
