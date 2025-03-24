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
	partition_struct->processes = list_create();

	// TODO : add : status, attributs, communication ports
	partition_struct->time_window = 1000;

	if (!partition_struct->stack)
		krnl_panic(ERR_STACK_ALLOC);

	CRITICAL_LEAVE();

	*partition_id = partition_struct->id;
	memset(partition_struct->stack, 0x69, stack_size);
	memset(partition_struct->stack, 0x33, 4);
	memset((partition_struct->stack) + stack_size - 4, 0x33, 4);

	_context_init(&partition_struct->context, (size_t)partition_struct->stack,
				  stack_size, (size_t)task);

	printf("MOS TASK : task %d: 0x%p, stack: 0x%p, size %d\n", partition_struct->id,
		   partition_struct->task, partition_struct->stack, partition_struct->stack_sz);

	partition_struct->state = TASK_READY;

	*return_code = NO_ERROR;
}

void add_new_partition(system_address_type *app_adress, partition_id_type *partition_id, return_code_type *return_code)
{
	// TODO -Q revoir création de task
	void *task = malloc(sizeof(void));
	if(!task) {
		*return_code = NOT_AVAILABLE;
		return;
	}
	task = app_adress;	
	// TODO -Q vérifier la taille
	stack_size_type stack_size = 1024;

	partition_spawn(task, stack_size, app_adress, partition_id, return_code);

	uint16_t mos_id = kcb->mos_struct->id;
	struct mos_s *mos_struct = list_foreach(kcb->tasks, idcmp, (void *)(size_t)mos_id)->data;
	list_insert(mos_struct->partitions, mos_struct->partitions->head, partition_id);
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
