#include "os.h"

/* defined in entry.S */
extern void switch_to(struct context *next);
extern void timer_load(int interval);

#define MAX_TASKS 10
#define STACK_SIZE 1024
#define TIMER_INTERVAL CLINT_TIMEBASE_FREQ

uint8_t task_stack[MAX_TASKS][STACK_SIZE];
struct context ctx_tasks[MAX_TASKS];
struct context *ctx_now;
struct task tasks[MAX_TASKS];

/*
 * _top is used to mark the max available position of ctx_tasks
 * _current is used to point to the context of current task
 */
static int _top = 0;
static int _current = -1;

void sched_init()
{
	w_mscratch(0);
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

	if (_current == -1) {
		_current = (_current + 1) % _top;
		timer_load(TIMER_INTERVAL * tasks[_current].timeslice);
		struct context *next = &(ctx_tasks[_current]);
		switch_to(next);
	}
	else {
		int next_task = (_current + 1) % _top;

		while (tasks[next_task].state == 's') {
			_current = (_current + 1) % _top;
			next_task = (_current + 1) % _top;
		}
		if (tasks[next_task].priority > tasks[_current].priority) 
			_current = -1;
		_current = (_current + 1) % _top;
		timer_load(TIMER_INTERVAL * tasks[_current].timeslice);
		task_go(_current);
	}
}

// switch to task[i]
void task_go(int i) 
{
	ctx_now = &ctx_tasks[i];
	switch_to(ctx_now);
}

/*
 * DESCRIPTION
 * 	Create a task.
 * 	- start_routin: task routune entry
 * RETURN VALUE
 * 	0: success
 * 	-1: if error occured
 */
int task_create(void (*task)(void* param), void *param, uint8_t priority, uint8_t timeslice)
{
	if (_top < MAX_TASKS) {

		if (_top == 0) {
			ctx_tasks[_top].sp = (reg_t) &task_stack[_top][STACK_SIZE - 1];
			ctx_tasks[_top].epc = (reg_t) task;
			tasks[_top].priority = priority;
			tasks[_top].timeslice = timeslice;
			tasks[_top].state = 'r';
		}
		else {
			int i;
			for (i = _top - 1; i >= 0; i--) {

				if (tasks[i].priority <= priority) break;

				ctx_tasks[i+1].sp = ctx_tasks[i].sp;
				ctx_tasks[i+1].epc = ctx_tasks[i].epc;
				tasks[i+1].timeslice = tasks[i].timeslice;
				tasks[i+1].priority = tasks[i].priority;
			}
			ctx_tasks[i+1].sp = (reg_t) &task_stack[_top][STACK_SIZE - 1];
			ctx_tasks[i+1].epc = (reg_t) task;
			tasks[i+1].priority = priority;
			tasks[i+1].timeslice = timeslice;
			tasks[i+1].state = 'r';
		}

		_top++;
		return 0;
	} else {
		return -1;
	}
}

/*
 * a very rough implementaion, just to consume the cpu
 */
void task_delay(volatile int count)
{
	count *= 50000;
	while (count--);
}

void task_exit(void)
{
	tasks[_current].state = 's';
	/* trigger a machine-level software interrupt */
	int id = r_mhartid();
	*(uint32_t*)CLINT_MSIP(id) = 1;
}


