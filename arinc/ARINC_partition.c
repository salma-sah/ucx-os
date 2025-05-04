#include <ucx.h>

void activate_current_partition_processes()
{
    struct partition_s *partition = kcb->mos_struct->current_partition->data;
    if (!partition) return;

	printf("PARTITION %d ACTIVATED\n", partition->id);
    // struct node_s *node = partition->processes->head;
    // while (node) {
    //     process_id_type process_id = (process_id_type)node->data;
    //     ucx_task_resume(process_id);
    //     node = node->next;
    // }
}

void deactivate_current_partition_processes()
{
    struct partition_s *partition = kcb->mos_struct->current_partition->data;
    if (!partition) return;

    struct node_s *node = partition->processes->head;
    while (node) {
        process_id_type process_id = (process_id_type)node->data;
        ucx_task_suspend(process_id);
        node = node->next;
    }
}

struct partition_s* partition_spawn(void *task, uint16_t stack_size, system_address_type *app_adress, uint16_t *partition_id, return_code_type *return_code)
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
	partition_struct->delay = 1;
	partition_struct->stack_sz = stack_size;
	partition_struct->id = kcb->id_next++;
	partition_struct->state = TASK_STOPPED;

	// TODO -Q vérifier prio
	partition_struct->priority = TASK_REALTIME_PRIO;
	partition_struct->rt_prio = 0;
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

	// printf("PARTITION TASK : task %d: 0x%p, stack: 0x%p, size %d\n", partition_struct->id,
	// 	   partition_struct->task, partition_struct->stack, partition_struct->stack_sz);

	partition_struct->state = TASK_STOPPED;
	return partition_struct;

	*return_code = NO_ERROR;
}

void add_new_partition(partition_id_type *partition_id, return_code_type *return_code)
{
	// TODO -Q revoir création de task
	void *task = (void*)activate_current_partition_processes;
	if(!task) {
		*return_code = NOT_AVAILABLE;
		return;
	}
	system_address_type *app_adress = task;

	// TODO -Q vérifier la taille
	stack_size_type stack_size = 1024;
	
	struct partition_s* partition_created = partition_spawn(task, stack_size, app_adress, partition_id, return_code);

	list_insert(kcb->mos_struct->partitions, kcb->mos_struct->partitions->head, partition_created);
	printf("Partition added lEN %d\n", kcb->mos_struct->partitions->length);
}

void get_partition_status(partition_status_type *partition_status, return_code_type *return_code)
{
}

void set_partition_mode(operating_mode_type operating_mode, return_code_type *return_code)
{
}

void get_my_partition_id(partition_id_type *partition_id, return_code_type *return_code)
{
	struct partition_s* my_partition = kcb->mos_struct->current_partition->data;
	partition_id = (partition_id_type*) my_partition->id;
	if (partition_id)
		*return_code = NO_ERROR;
	else
		*return_code = NOT_AVAILABLE;
}
