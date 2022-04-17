#include "os.h"

/* defined in entry.S */
extern void switch_to(struct context *next);

#define MAX_TASKS 10
#define STACK_SIZE 1024
uint8_t task_stack[MAX_TASKS][STACK_SIZE];
struct context ctx_tasks[MAX_TASKS];
int delay[MAX_TASKS];
struct task tasks[MAX_TASKS];
struct context ctx_os;
uint8_t os_stack[STACK_SIZE];

/*
 * _top is used to mark the max available position of ctx_tasks
 * _current is used to point to the context of current task
 */
static int _top = 0;
static int _current = -1;

void task_os()
{
	while (1) {}
}

void sched_init()
{
	w_mscratch((reg_t)&ctx_os);
	ctx_os.sp = (reg_t) &os_stack[STACK_SIZE - 1];
	ctx_os.pc = (reg_t) task_os;

	/* enable machine-mode software interrupts. */
	w_mie(r_mie() | MIE_MSIE);
}

/*
 * implment a simple cycle FIFO schedular
 */
void schedule()
{
	if (_top <= 0) {
		panic("Num of task should be greater than zero!");
		return;
	}

	_current = (_current + 1) % _top;
	if (tasks[_current].state == 'w') {
		int i = _current;
		do {
			if (tasks[i].state != 'w') break;
			i = (i + 1) % _top;
		} while (i != _current);
		if (i == _current) switch_to(&ctx_os);
	}
	struct context *next = &(ctx_tasks[_current]);
	switch_to(next);
}

/*
 * DESCRIPTION
 * 	Create a task.
 * 	- start_routin: task routune entry
 * RETURN VALUE
 * 	0: success
 * 	-1: if error occured
 */
int task_create(void (*start_routin)(void))
{
	if (_top < MAX_TASKS) {
		ctx_tasks[_top].sp = (reg_t) &task_stack[_top][STACK_SIZE - 1];
		ctx_tasks[_top].pc = (reg_t) start_routin;
		tasks[_top].state = 'r';
		_top++;
		return 0;
	} else {
		return -1;
	}
}

/*
 * DESCRIPTION
 * 	task_yield()  causes the calling task to relinquish the CPU and a new 
 * 	task gets to run.
 */
void task_yield()
{
	/* trigger a machine-level software interrupt */
	int id = r_mhartid();
	*(uint32_t*)CLINT_MSIP(id) = 1;
}

void task_reready(void *arg)
{
	if (NULL == arg)
		return;
	tasks[*(int *)arg].state = 'r';
	printf("task reready\n");
}

/*
 * a very rough implementaion, just to consume the cpu
 */
void task_delay(uint32_t tick)
{
	// 移出操作
	// TODO()
	tasks[_current].state = 'w';
	delay[_current] = _current;

	// 移入操作
	timer *t1 = timer_create(task_reready, &(delay[_current]), tick);
	if (NULL == t1) {
		printf("timer_create() failed!\n");
	}

	task_yield();

}

