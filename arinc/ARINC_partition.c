#include <ucx.h>

static struct node_s *idcmp(struct node_s *node, void *id_arg)
{
    struct tcb_s *task = node->data;
    uint16_t id = (size_t)id_arg;

    if (task->id == id)
        return node;
    else
        return 0;
}

void create_mos(uint16_t *mos_id, return_code_type *return_code)
{
	// TODO -Q revoir création de task
	void *task = malloc(sizeof(void));

	// TODO -Q vérifier la taille
	stack_size_type stack_size = 4 * 1024 * 1024;

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

	mos_id = new_mos->id;
	memset(new_mos->stack, 0x69, stack_size);
	memset(new_mos->stack, 0x33, 4);
	memset((new_mos->stack) + stack_size - 4, 0x33, 4);

	_context_init(&new_mos->context, (size_t)new_mos->stack,
				  stack_size, (size_t)task);

	printf("MOS TASK : task %d: 0x%p, stack: 0x%p, size %d\n", new_mos->id,
		   new_mos->task, new_mos->stack, new_mos->stack_sz);

	new_mos->state = TASK_READY;

	return ERR_OK;
}

void partition_spawn(void *task, uint16_t stack_size, system_address_type *app_adress, uint16_t *partition_id, return_code_type *return_code)
{
	struct partition_s *partition_struct;
	struct node_s *new_partition;

	partition_struct = malloc(sizeof(struct partition_s));

	if (!partition_struct)
		krnl_panic(ERR_PARTITION_ALLOC);

	CRITICAL_ENTER();

	new_partition = list_pushback(kcb->tasks, partition_struct);

	if (!new_partition)
		krnl_panic(ERR_PARTITION_ALLOC);

	new_partition->data = partition_struct;
	partition_struct->task = task;
	partition_struct->delay = 0;
	partition_struct->stack_sz = stack_size;
	partition_struct->id = kcb->id_next++;
	partition_struct->state = TASK_STOPPED;

	// TODO -Q vérifier prio
	partition_struct->priority = TASK_REALTIME_PRIO;
	partition_struct->stack = malloc(stack_size);
	partition_struct->process = list_create();

	// TODO : add : status, attributs, communication ports

	if (!partition_struct->stack)
		krnl_panic(ERR_STACK_ALLOC);

	CRITICAL_LEAVE();

	partition_id = partition_struct->id;
	memset(partition_struct->stack, 0x69, stack_size);
	memset(partition_struct->stack, 0x33, 4);
	memset((partition_struct->stack) + stack_size - 4, 0x33, 4);

	_context_init(&partition_struct->context, (size_t)partition_struct->stack,
				  stack_size, (size_t)task);

	printf("MOS TASK : task %d: 0x%p, stack: 0x%p, size %d\n", partition_struct->id,
		   partition_struct->task, partition_struct->stack, partition_struct->stack_sz);

	partition_struct->state = TASK_READY;

	return ERR_OK;
}

void add_new_partition(uint16_t mos_id, system_address_type *app_adress, partition_id_type *partition_id, return_code_type *return_code)
{
	// TODO -Q revoir création de task
	void *task = malloc(sizeof(void));

	// TODO -Q vérifier la taille
	stack_size_type stack_size = 1024 * 1024;

	partition_spawn(task, stack_size, app_adress, partition_id, return_code);

	struct mos_s *mos_struct = list_foreach(kcb->tasks, idcmp, (void *)(size_t)mos_id)->data;
	list_insert(mos_struct->partitions, mos_struct->partitions->tail, partition_id);
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

void get_partition_status(partition_status_type *partition_status, return_code_type *return_code)
{
}

void set_partition_mode(operating_mode_type operating_mode, return_code_type *return_code)
{
}

void get_my_partition_id(partition_id_type *partition_id, return_code_type *return_code)
{
}
