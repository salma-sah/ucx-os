#include <ARINC_apex_types.h>

/*--------------------------------------------------------------------*/
/*                                                                    */
/* sampling port constant and type definitions and management services*/
/*                                                                    */
/*--------------------------------------------------------------------*/

#ifndef apex_sampling
#define apex_sampling

#define max_number_of_sampling_ports system_limit_number_of_sampling_ports

typedef name_type sampling_port_name_type;

typedef apex_integer sampling_port_id_type;
#define null_sampling_port_id 0

typedef enum
{
       invalid = 0,
       valid = 1
} validity_type;

typedef struct
{
       system_time_type refresh_period;
       message_size_type max_message_size;
       port_direction_type port_direction;
       validity_type last_msg_validity;
} sampling_port_status_type;

extern void create_sampling_port(
    /*in */ sampling_port_name_type sampling_port_name,
    /*in */ message_size_type max_message_size,
    /*in */ port_direction_type port_direction,
    /*in */ system_time_type refresh_period,
    /*out*/ sampling_port_id_type *sampling_port_id,
    /*out*/ return_code_type *return_code);

extern void write_sampling_message(
    /*in */ sampling_port_id_type sampling_port_id,
    /*in */ message_addr_type message_addr, /* by reference */
    /*in */ message_size_type length,
    /*out*/ return_code_type *return_code);

extern void read_sampling_message(
    /*in */ sampling_port_id_type sampling_port_id,
    /*in */ message_addr_type message_addr,
    /* the message address is passed in, although */
    /* the respective message is passed out       */
    /*out*/ message_size_type *length,
    /*out*/ validity_type *validity,
    /*out*/ return_code_type *return_code);

extern void get_sampling_port_id(
    /*in */ sampling_port_name_type sampling_port_name,
    /*out*/ sampling_port_id_type *sampling_port_id,
    /*out*/ return_code_type *return_code);

extern void get_sampling_port_status(
    /*in */ sampling_port_id_type sampling_port_id,
    /*out*/ sampling_port_status_type *sampling_port_status,
    /*out*/ return_code_type *return_code);

#endif
