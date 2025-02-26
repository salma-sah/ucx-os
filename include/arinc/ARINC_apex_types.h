#include <stdbool.h>
/*----------------------------------------------------------------*/
/*                                                                */
/* global constant and type definitions                           */
/*                                                                */
/*----------------------------------------------------------------*/

#ifndef apex_types
#define apex_types

/*---------------------------*/
/* domain limits             */
/*---------------------------*/

/*                       implementation dependent                          */
/* these values define the domain limits and are implementation dependent. */
/* these values represent typical minimum values supported by an o/s,      */
/* unless otherwise limited (e.g., message size limited by transport).     */
/* for partition scope, the limits are per partition.                      */

#define system_limit_number_of_partitions 32 /* module scope */

#define system_limit_number_of_messages 512 /* module scope */

#define system_limit_message_size 8192 /* module scope */

#define system_limit_number_of_processes 128 /* partition scope */

#define system_limit_number_of_sampling_ports 512 /* partition scope */

#define system_limit_number_of_queuing_ports 512 /* partition scope */

#define system_limit_number_of_buffers 256 /* partition scope */

#define system_limit_number_of_blackboards 256 /* partition scope */

#define system_limit_number_of_semaphores 256 /* partition scope */

#define system_limit_number_of_events 256 /* partition scope */

#define system_limit_number_of_mutexes 256 /* partition scope */

/*----------------------*/
/* base APEX types      */
/*----------------------*/

/*                       implementation portable                         */
/*  the sizes of these base types are identical for all systems and the  */
/*  actual types must match the types used by the implementation of the  */
/*  underlying operating system to result in the defined sizes.          */

typedef unsigned char apex_byte; /* 8-bit unsigned  */
typedef int apex_integer; /* 32-bit signed   */
typedef unsigned apex_unsigned; /* 32-bit unsigned */
typedef long long apex_long_integer; /* 64-bit signed   */
typedef bool apex_boolean; /* 64-bit signed   */

/*  the following types are either 32-bit or 64-bit and will match */
/*  the processor's native address size.                           */
typedef long apex_native_integer;
typedef unsigned long apex_native_unsigned;

/*----------------------*/
/* general APEX types   */
/*----------------------*/

typedef enum
{
   NO_ERROR = 0,       /* request valid and operation performed      */
   NO_ACTION = 1,      /* status of system unaffected by request     */
   NOT_AVAILABLE = 2,  /* resource required by request unavailable   */
   INVALID_PARAM = 3,  /* invalid parameter specified in request     */
   INVALID_CONFIG = 4, /* parameter incompatible with configuration  */
   INVALID_MODE = 5,   /* request incompatible with current mode     */
   TIMED_OUT = 6       /* time-out tied up with request has expired  */
} return_code_type;

#define max_name_length 32

typedef char name_type[max_name_length];

/* the size of address pointers must match the address range supported by   */
/* the processor (e.g., 32-bits or 64-bits as supported or as configured    */
/* for a processor).                                                        */
typedef void(*system_address_type);
typedef apex_byte *message_addr_type;
typedef apex_integer message_size_type;
typedef apex_integer message_range_type;

typedef enum
{
   source = 0,
   destination = 1
} port_direction_type;

typedef enum
{
   fifo = 0,
   priority = 1
} queuing_discipline_type;

typedef apex_long_integer system_time_type;
/* 64-bit signed integer with a 1 nanosecond lsb */

#define infinite_time_value -1

typedef apex_integer processor_core_id_type;

// TODO review type
#define core_affinity_no_preference <implementation dependent>

#endif
