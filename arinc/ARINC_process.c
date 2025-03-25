#include <ucx.h>
#include <limits.h>

static struct node_s *find_task(struct node_s *node, void *id_arg)
{
    struct tcb_s *task = node->data;
    uint16_t id = (size_t)id_arg;

    if (task->id == id)
        return node;
    else
        return 0;
}

static struct node_s *find_partition(struct node_s *node, void *id_arg)
{
    partition_id_type id = *(partition_id_type*)node->data;
    partition_id_type id_to_find = (size_t)id_arg;

    printf("ID original %d\n", id);
    printf("ID to find %d\n", id_to_find);
    if (id_to_find == id)
        return node;
    else
        return 0;
}

void create_process(process_attribute_type *attributes, partition_id_type partition_id, process_id_type *process_id, return_code_type *return_code)
{
    //1 TODO tenir compte des autres process de la partition

    if (attributes->stack_size > UINT_MAX 
        || attributes->base_priority < INT_MIN 
        || attributes->base_priority > INT_MAX 
        || attributes->period < LLONG_MIN
        || attributes->period > LLONG_MAX
        || attributes->time_capacity < LLONG_MIN
        || attributes->time_capacity > LLONG_MAX){
            *return_code = INVALID_PARAM;
            return;
        }

    if (attributes->time_capacity > attributes->period) {
        *return_code = INVALID_PARAM;
        return;
    }
    int32_t err_code = ucx_task_spawn(attributes->entry_point, attributes->stack_size);

    if (err_code != ERR_OK) {
        *return_code = NOT_AVAILABLE;
        return;
    }

    struct process_s *new_process = malloc(sizeof(struct process_s));
    *process_id = ucx_task_idref(attributes->entry_point);

    new_process->process_id = *process_id;
    new_process->attributes = attributes;

    size_t index_next = kcb->id_next;
    new_process->process_index = --index_next;

    struct tcb_s *process_tcb = list_foreach(kcb->tasks, find_task, (void *)(size_t)*process_id)->data;

    // TODO -Q revoir les conversions de types
    process_tcb->priority = (uint16_t)attributes->base_priority;

    //2 TODO -Q revoir attributs restants

    process_status_type *process_status = malloc(sizeof(process_status_type));
    process_status->process_state = DORMANT;
    process_status->deadline_time = attributes->time_capacity;
    process_status->current_priority = attributes->base_priority;
    new_process->processus_status = process_status;

	struct partition_s *partition = list_foreach(kcb->mos_struct->partitions, find_partition, (void *)(size_t)partition_id)->data;
    printf("PARTITION FOUND\n");
    list_insert(partition->processes, partition->processes->head, process_id);
    printf("PROCESS ADDED\n");
    // TODO revoir process_core_id
    initialize_process_core_affinity(*process_id, 0, return_code);
}

void set_priority(process_id_type process_id, priority_type priority, return_code_type *return_code)
{
}

void suspend_self(system_time_type time_out, return_code_type *return_code)
{
    uint16_t process_id = ucx_task_id();
    ucx_task_suspend(process_id);
}

void suspend(process_id_type process_id, return_code_type *return_code)
{
}

void resume(process_id_type process_id, return_code_type *return_code)
{
}

// TODO -Q équivalent à cancel ?
void stop_self(void)
{
}

void stop(process_id_type process_id, return_code_type *return_code)
{
}

void start(process_id_type process_id, return_code_type *return_code)
{
}

void delayed_start(process_id_type process_id, system_time_type delay_time, return_code_type *return_code)
{
}

void lock_preemption(lock_level_type *lock_level, return_code_type *return_code)
{
}

void unlock_preemption(lock_level_type *lock_level, return_code_type *return_code)
{
}

void get_my_id(process_id_type *process_id, return_code_type *return_code)
{
}

void get_process_id(process_name_type process_name, process_id_type *process_id, return_code_type *return_code)
{
}

void get_process_status(process_id_type process_id, process_status_type *process_status, return_code_type *return_code)
{
}

void initialize_process_core_affinity(process_id_type process_id, processor_core_id_type processor_core_id, return_code_type *return_code)
{
    *return_code = NO_ERROR;
}

void get_my_processor_core_id(processor_core_id_type *processor_core_id, return_code_type *return_code)
{
}

void get_my_index(process_index_type *process_index, return_code_type *return_code)
{
}
