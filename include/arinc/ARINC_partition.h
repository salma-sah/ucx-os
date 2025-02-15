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

    typedef enum {
           normal_start = 0,
           partition_restart = 1,
           hm_module_restart = 2,
           hm_partition_restart = 3
    } start_condition_type;

typedef struct
{
       system_time_type period;
       system_time_type duration;
       partition_id_type identifier;
       lock_level_type lock_level;
       operating_mode_type operating_mode;
       start_condition_type start_condition;
       num_cores_type num_assigned_cores;
} partition_status_type;

extern void get_partition_status(
    /*out*/ partition_status_type *partition_status,
    /*out*/ return_code_type *return_code);

extern void set_partition_mode(
    /*in */ operating_mode_type operating_mode,
    /*out*/ return_code_type *return_code);

extern void get_my_partition_id(
    /*out*/ partition_id_type *partition_id,
    /*out*/ return_code_type *return_code);

#endif
