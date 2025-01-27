#include "platform.h"

	# size of each hart's stack is 1024 bytes
	.equ	STACK_SIZE, 1024

	.global	_start

	.text
_start:
	# park harts with id != 0
	csrr	t0, mhartid		# read current hart id
	mv	tp, t0			# keep CPU's hartid in its tp for later usage.
	bnez	t0, park		# if we're not on the hart 0
					# we park the hart

	# Set all bytes in the BSS section to zero.
	la	a0, _bss_start
	la	a1, _bss_end
	bgeu	a0, a1, 2f
1:
	sw	zero, (a0)
	addi	a0, a0, 4
	bltu	a0, a1, 1b
2:
	# Setup stacks, the stack grows from bottom to top, so we put the
	# stack pointer to the very end of the stack range.
	slli	t0, t0, 10		# shift left the hart id by 1024
	la	sp, stacks + STACK_SIZE	# set the initial stack pointer
					# to the end of the first stack space
	add	sp, sp, t0		# move the current hart stack pointer
					# to its place in the stack space

	# start_kernel 中第一次调用 schedule() 最后在 switch to 第一个任务中会调用 MRET
	# 因此需要在此处首先初始化 mstatus
	# 注意: mstatus 默认值为 0
	# MPP 保存发生中断前权限级别值
	# mstatus.MPP 设置为 3, 当 MRET 调用后返回 MATCHINE 模式
	# MPIE 保存发生中断前 XIE 全局中断的值
	# mstatus.MPIE 设置为 1, MRET 会启用中断
	li	t0, 3 << 11 | 1 << 7
	csrr	a1, mstatus
	or	t0, t0, a1
	csrw	mstatus, t0

	j	start_kernel		# hart 0 jump to c

park:
	wfi
	j	park

stacks:
	.skip	STACK_SIZE * MAXNUM_CPU # allocate space for all the harts stacks

	.end				# End of file
