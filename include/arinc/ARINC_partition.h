#include "ARINC_apex_types.h"

/*----------------------------------------------------------------*/
/*                                                                */
/* Partition constant and type definitions and management services*/
/*                                                                */
/*----------------------------------------------------------------*/

#ifndef apex_partition
#define apex_partition

#define max_number_of_partitions system_limit_number_of_partitions

typedef enum
{
    idle = 0,
    cold_start = 1,
    warm_start = 2,
    normal = 3
} operating_mode_type;

/* partition_ids originate from the configuration data */
typedef uint16_t partition_id_type;
typedef apex_unsigned num_cores_type;
typedef apex_integer lock_level_type;
typedef name_type partition_name_type;
typedef apex_boolean system_partition_type;

typedef enum
{
    normal_start = 0,
    partition_restart = 1,
    hm_module_restart = 2,
    hm_partition_restart = 3
} start_condition_type;

/* MOS node */
struct mos_s
{
    void (*task)(void);
    jmp_buf context; /* jmp_buf is architecture specific */
    size_t *stack;
    size_t stack_sz;
    void *rt_prio;
    uint16_t id;
    uint16_t delay;
    uint16_t priority;
    uint8_t state;
    struct list_s *partitions;
};

typedef struct
{
    lock_level_type lock_level;
    operating_mode_type operating_mode;
    start_condition_type start_condition;
} partition_status_type;

/*TODO - REMOVE Typically, the partition period is the greatest common factor of the process periods
within a partition. If the process periods are harmonic, then this is the period of the
process in a partition that has the shortest period. If there are no periodic processes
in a partition, then the period is based on the minimum execution frequency required
by an application to satisfy its performance requirements.*/
typedef struct
{
    system_time_type period;
    system_time_type duration;
    partition_id_type identifier;
    partition_name_type name;
    system_address_type entry_point;
    num_cores_type num_assigned_cores;
    system_partition_type is_system_partition;
} partition_attributs_type;

/* Partition struct */
// TODO add Partition Health Monitor Table
struct partition_s
{
    void (*task)(void);
    jmp_buf context; /* jmp_buf is architecture specific */
    size_t *stack;
    size_t stack_sz;
    void *rt_prio;
    uint16_t id;
    uint16_t delay;
    uint16_t priority;
    uint8_t state;
    partition_attributs_type *attributs;
    partition_status_type *status;
    struct list_s *process;
    sampling_port_s *communication_port;
};

extern void create_mos(
    /*out*/ uint16_t *mos_id,
    /*out*/ return_code_type *return_code);

extern void mos_spawn(
    /*in*/ void *task,
    /*in*/ uint16_t stack_size,
    /*out*/ uint16_t *mos_id,
    /*out*/ return_code_type *return_code);

extern void partition_spawn(
    /*in*/void *task, 
    /*in*/ uint16_t stack_size, 
    /*in*/ system_address_type *app_adress, 
    /*out*/ uint16_t *partition_id, 
    /*out*/ return_code_type *return_code);

extern void add_new_partition(
    /*in*/ uint16_t mos_id, 
    /*out*/ system_address_type* app_adress,
    /*out*/ partition_id_type* partition_id,
    /*out*/ return_code_type *return_code);

extern void trigger_cold_start_mode(
    /*in*/ partition_id_type partition_id,
    /*out*/ return_code_type *return_code);

extern void trigger_warm_start_mode(
    /*in*/ partition_id_type partition_id,
    /*out*/ return_code_type *return_code);

extern void trigger_normal_mode(
    /*in*/ partition_id_type partition_id,
    /*out*/ return_code_type *return_code);

extern void trigger_idle_mode(
    /*in*/ partition_id_type partition_id,
    /*out*/ return_code_type *return_code);

extern void get_partition_status(
    /*out*/ partition_status_type *partition_status,
    /*out*/ return_code_type *return_code);

extern void set_partition_mode(
    /*in*/ operating_mode_type operating_mode,
    /*out*/ return_code_type *return_code);

extern void get_my_partition_id(
    /*out*/ partition_id_type *partition_id,
    /*out*/ return_code_type *return_code);

#endif
