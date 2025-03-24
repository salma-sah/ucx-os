#include "ARINC_apex_types.h"
#include "ARINC_partition.h"
/*----------------------------------------------------------------*/
/*                                                                */
/* MOS constant and type definitions and management services*/
/*                                                                */
/*----------------------------------------------------------------*/

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
    struct partition_s* current_partition;
};

extern void create_mos(
    /*out*/ uint16_t *mos_id,
    /*out*/ return_code_type *return_code);

extern void mos_spawn(
    /*in*/ void *task,
    /*in*/ uint16_t stack_size,
    /*out*/ uint16_t *mos_id,
    /*out*/ return_code_type *return_code);

extern void schedule_partitions();

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
