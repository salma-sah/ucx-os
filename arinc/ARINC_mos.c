#include <ucx.h>

void create_mos(uint16_t *mos_id, return_code_type *return_code)
{
	// TODO -Q revoir création de task
	void *task = malloc(sizeof(void));

	// TODO -Q vérifier la taille
	stack_size_type stack_size = 65535;

	mos_spawn(task, stack_size, mos_id, return_code);
}

void mos_spawn(void *task, uint16_t stack_size, uint16_t *mos_id, return_code_type *return_code)
{
	struct mos_s *new_mos;
	struct node_s *new_task;

	new_mos = malloc(sizeof(struct mos_s));

	if (!new_mos)
		krnl_panic(ERR_MOS_ALLOC);

	CRITICAL_ENTER();

	new_task = list_pushback(kcb->tasks, new_mos);

	if (!new_task)
		krnl_panic(ERR_MOS_ALLOC);

	new_task->data = new_mos;
	new_mos->task = task;
	new_mos->delay = 0;
	new_mos->stack_sz = stack_size;
	new_mos->id = kcb->id_next++;
	new_mos->state = TASK_STOPPED;

	// TODO -Q vérifier prio
	new_mos->priority = TASK_REALTIME_PRIO;
	new_mos->stack = malloc(stack_size);
	new_mos->partitions = list_create();

	if (!new_mos->stack)
		krnl_panic(ERR_STACK_ALLOC);

	CRITICAL_LEAVE();

	*mos_id = new_mos->id;
	memset(new_mos->stack, 0x69, stack_size);
	memset(new_mos->stack, 0x33, 4);
	memset((new_mos->stack) + stack_size - 4, 0x33, 4);

	_context_init(&new_mos->context, (size_t)new_mos->stack,
				  stack_size, (size_t)task);

	printf("MOS TASK : task %d: 0x%p, stack: 0x%p, size %d\n", new_mos->id,
		   new_mos->task, new_mos->stack, new_mos->stack_sz);

	new_mos->state = TASK_READY;

	*return_code = NO_ERROR;
}

void trigger_cold_start_mode(partition_id_type partition_id, return_code_type *return_code)
{
}

void trigger_warm_start_mode(partition_id_type partition_id, return_code_type *return_code)
{
}

void trigger_normal_mode(partition_id_type partition_id, return_code_type *return_code)
{
}

void trigger_idle_mode(partition_id_type partition_id, return_code_type *return_code)
{
}
