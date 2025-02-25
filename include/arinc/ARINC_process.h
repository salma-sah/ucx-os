#include <ARINC_apex_types.h>

/*----------------------------------------------------------------*/
/*                                                                */
/* process constant and type definitions and management services  */
/*                                                                */
/*----------------------------------------------------------------*/

#ifndef apex_process
#define apex_process

#define max_number_of_processes system_limit_number_of_processes

#define min_priority_value 1

#define max_priority_value 239

#define max_lock_level 16

typedef name_type process_name_type;

typedef apex_integer process_id_type;
#define null_process_id 0

// TODO review main process id value
#define main_process_id 1

typedef apex_integer process_index_type;

typedef apex_integer lock_level_type;

typedef apex_unsigned stack_size_type;

typedef apex_integer waiting_range_type;

typedef apex_integer priority_type;

typedef enum
{
    DORMANT = (uint8_t)0,
    READY = (uint8_t)1,
    RUNNING = (uint8_t)2,
    WAITING = (uint8_t)3,
    FAULTED = (uint8_t)4
} process_state_type;

typedef enum
{
    soft = 0,
    hard = 1
} deadline_type;

typedef struct
{
    system_time_type period;
    system_time_type time_capacity;
    system_address_type entry_point;
    stack_size_type stack_size;
    priority_type base_priority;
    deadline_type deadline;
    process_name_type name;
} process_attribute_type;

typedef struct
{
    system_time_type deadline_time;
    priority_type current_priority;
    process_state_type process_state;
} process_status_type;

struct process_s
{
    process_status_type *processus_status;
    process_attribute_type *attributes;
    process_id_type process_id;
    process_index_type process_index;
};

extern void create_process(
    /*in */ process_attribute_type *attributes,
    /*out*/ process_id_type *process_id,
    /*out*/ return_code_type *return_code);

extern void set_priority(
    /*in */ process_id_type process_id,
    /*in */ priority_type priority,
    /*out*/ return_code_type *return_code);

extern void suspend_self(
    /*in */ system_time_type time_out,
    /*out*/ return_code_type *return_code);

extern void suspend(
    /*in */ process_id_type process_id,
    /*out*/ return_code_type *return_code);

extern void resume(
    /*in */ process_id_type process_id,
    /*out*/ return_code_type *return_code);

extern void stop_self(void);

extern void stop(
    /*in */ process_id_type process_id,
    /*out*/ return_code_type *return_code);

extern void start(
    /*in */ process_id_type process_id,
    /*out*/ return_code_type *return_code);

extern void delayed_start(
    /*in */ process_id_type process_id,
    /*in */ system_time_type delay_time,
    /*out*/ return_code_type *return_code);

extern void lock_preemption(
    /*out*/ lock_level_type *lock_level,
    /*out*/ return_code_type *return_code);

extern void unlock_preemption(
    /*out*/ lock_level_type *lock_level,
    /*out*/ return_code_type *return_code);

extern void get_my_id(
    /*out*/ process_id_type *process_id,
    /*out*/ return_code_type *return_code);

extern void get_process_id(
    /*in */ process_name_type process_name,
    /*out*/ process_id_type *process_id,
    /*out*/ return_code_type *return_code);

extern void get_process_status(
    /*in */ process_id_type process_id,
    /*out*/ process_status_type *process_status,
    /*out*/ return_code_type *return_code);

extern void initialize_process_core_affinity(
    /*in */ process_id_type process_id,
    /*in */ processor_core_id_type processor_core_id,
    /*out*/ return_code_type *return_code);

extern void get_my_processor_core_id(
    /*out*/ processor_core_id_type *processor_core_id,
    /*out*/ return_code_type *return_code);

extern void get_my_index(
    /*out*/ process_index_type *process_index,
    /*out*/ return_code_type *return_code);

#endif
