#include <ARINC_apex_types.h>

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
typedef apex_integer partition_id_type;
typedef apex_unsigned num_cores_type;
typedef apex_integer lock_level_type;
typedef name_type partition_name_type;

typedef enum
{
    normal_start = 0,
    partition_restart = 1,
    hm_module_restart = 2,
    hm_partition_restart = 3
} start_condition_type;

typedef struct
{
    lock_level_type lock_level;
    operating_mode_type operating_mode;
    num_cores_type num_assigned_cores;
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
    start_condition_type start_condition;
} partition_attributs_type;



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

/* Partition struct */
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

extern void create_mos(
    /*out*/ uint16_t *mos_id,
    /*out*/ return_code_type *return_code);

extern void mos_spawn(
    /*in*/ void *task,
    /*in*/ uint16_t stack_size,
    /*out*/ uint16_t *mos_id,
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
