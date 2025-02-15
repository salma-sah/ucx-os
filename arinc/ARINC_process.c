#include <ucx.h>

void create_process(process_attribute_type *attributes, process_id_type *process_id, return_code_type *return_code){
    /*
    system_time_type period;
    system_time_type time_capacity;
    system_address_type entry_point;
    stack_size_type stack_size;
    priority_type base_priority;
    deadline_type deadline;
    process_name_type name;
    */
   
    /*
    create a new process with the process attributes set to ATTRIBUTES; 
    
    set the process state to DORMANT; 
    initialize process context, unique process index, and stack; 
    set the process’s core affinity to the default process core affinity value; 
    PROCESS_ID  := unique identifier assigned by the O/S to the created process; 
    RETURN_CODE := NO_ERROR; 
    

    int32_t err_code = ucx_task_spawn(task, attributes->stack_size);

    if (err_code == ERR_OK) {

    }
    */

    
}

void set_priority(process_id_type process_id, priority_type priority, return_code_type *return_code){

}

void suspend_self(system_time_type time_out, return_code_type *return_code){

}

void suspend(process_id_type process_id, return_code_type *return_code){

}

void resume(process_id_type process_id, return_code_type *return_code){

}

void stop_self(void){

}

void stop(process_id_type process_id, return_code_type *return_code){

}

void start(process_id_type process_id, return_code_type *return_code){

}

void delayed_start(process_id_type process_id, system_time_type delay_time, return_code_type *return_code){

}

void lock_preemption(lock_level_type *lock_level, return_code_type *return_code){

}

void unlock_preemption(lock_level_type *lock_level, return_code_type *return_code){

}

void get_my_id(process_id_type *process_id, return_code_type *return_code){

}

void get_process_id(process_name_type process_name, process_id_type *process_id, return_code_type *return_code){

}

void get_process_status(process_id_type process_id, process_status_type *process_status, return_code_type *return_code){

}

void initialize_process_core_affinity(process_id_type process_id, processor_core_id_type processor_core_id, return_code_type *return_code){

}

void get_my_processor_core_id(processor_core_id_type *processor_core_id, return_code_type *return_code){

}

void get_my_index(process_index_type *process_index, return_code_type *return_code){

}
