#include <ucx.h>
static struct node_s *find_timer(struct node_s *node, void *id_arg)
{
    struct timer_s *timer = node->data;
    uint16_t id = (size_t)id_arg;

    if (timer->timer_id == id)
        return node;
    else
        return 0;
}

void create_mos(uint16_t *mos_id, return_code_type *return_code)
{
	void *task = (void*)schedule_partitions;

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

	kcb->rt_sched = mos_execution;
	new_task = list_pushback(kcb->tasks, new_mos);

	if (!new_task)
		krnl_panic(ERR_MOS_ALLOC);

	new_task->data = new_mos;
	new_mos->task = task;
	new_mos->delay = 0;
	new_mos->stack_sz = stack_size;
	new_mos->id = kcb->id_next++;

	kcb->mos_struct = new_mos;

	new_mos->state = TASK_STOPPED;

	// TODO -Q vérifier prio
	new_mos->priority = TASK_REALTIME_PRIO;
	new_mos->rt_prio = 1;
	new_mos->stack = malloc(stack_size);
	new_mos->partitions = list_create();
	new_mos->current_partition = NULL;

	if (!new_mos->stack)
		krnl_panic(ERR_STACK_ALLOC);

	CRITICAL_LEAVE();

	*mos_id = new_mos->id;
	memset(new_mos->stack, 0x69, stack_size);
	memset(new_mos->stack, 0x33, 4);
	memset((new_mos->stack) + stack_size - 4, 0x33, 4);

	_context_init(&new_mos->context, (size_t)new_mos->stack,
				  stack_size, (size_t)task);

	// printf("MOS TASK : task %d: 0x%p, stack: 0x%p, size %d\n", new_mos->id,
	// 	   new_mos->task, new_mos->stack, new_mos->stack_sz);

	new_mos->state = TASK_READY;
	*return_code = NO_ERROR;
}

void *partition_timer_cb(void *arg)
{
    partition_id_type partition_id = *(partition_id_type *)arg;
	deactivate_current_partition_processes(partition_id);
	ucx_task_suspend(partition_id);
    printf("Partition %d time window expired\n", partition_id);
	return NULL;
}

void mos_execution() {
	while(true)
		schedule_partitions();
}

int32_t schedule_partitions()
{
    struct list_s *partitions = kcb->mos_struct->partitions;
    if (!partitions) {
		return -1;
	}

	struct tcb_s *partition_task = kcb->task_current->data;
	if (partition_task->state == TASK_RUNNING)
		partition_task->state = TASK_READY;
	
	struct node_s* partition_node;
	struct partition_s* partition;

	if (!kcb->mos_struct->current_partition) {
		kcb->mos_struct->current_partition = partitions->head;
		partition_node = partitions->head;
		partition = partition_node->data;
		partition->timer_id = ucx_timer_create(partition_timer_cb, partition->time_window);
		ucx_timer_start(partition->timer_id, TIMER_ONESHOT);
	}
	else {
		partition_node = kcb->mos_struct->current_partition;
		partition = partition_node->data;
		struct timer_s* timer = list_foreach(kcb->timer_lst, find_timer, (void *)(size_t) partition->timer_id)->data;
		if (timer->countdown == 0){
			if (kcb->mos_struct->current_partition->next != partitions->tail) {
				kcb->mos_struct->current_partition = kcb->mos_struct->current_partition->next;
				partition_node = kcb->mos_struct->current_partition->next;
				partition = partition_node->data;
				kcb->mos_struct->current_partition = partition_node;
			}
			else {
				kcb->mos_struct->current_partition = partitions->head;
				partition_node = partitions->head;
				partition = partition_node->data;
				kcb->mos_struct->current_partition = partition_node;
			}
			
		}
	}

	kcb->task_current = partition_node;
	partition_task->state = TASK_RUNNING;
	
	return partition->id;
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
