#include <ucx.h>
void test1(){
    printf("PROCESS PARTITION 1111111111111\n");
}
void test2(){
    printf("PROCESS PARTITION 2222222222222\n");
}
void test_create_process1(partition_id_type partition_id) {
    process_attribute_type attributes;
    process_id_type process_id;
    return_code_type return_code;
    attributes.stack_size = 1024;
    attributes.base_priority = 10;
    attributes.period = 1000;
    attributes.time_capacity = 500;
    attributes.entry_point = (void*)test1;
    create_process(&attributes, partition_id, &process_id, &return_code);
    printf("Test 1111111111 : return_code = %d\n", return_code);
}

void test_create_process2(partition_id_type partition_id) {
    process_attribute_type attributes;
    process_id_type process_id;
    return_code_type return_code;
    attributes.stack_size = 1024;
    attributes.base_priority = 10;
    attributes.period = 1000;
    attributes.time_capacity = 500;
    attributes.entry_point = (void*)test2;
    create_process(&attributes, partition_id, &process_id, &return_code);
    printf("Test 2222222222222 : return_code = %d\n", return_code);
}

void test_mos_and_partitions_creation() {
	uint16_t mos_id;
	return_code_type return_code;
	partition_id_type first_partition_id;
    partition_id_type second_partition_id;

    create_mos(&mos_id, &return_code);
    if (return_code != NO_ERROR)
    {
        printf("[Error] Failed to create MOS\n");
        return;
    }
    printf("[Success] MOS created with ID: %d\n", mos_id);
    
	add_new_partition(&first_partition_id, &return_code);
    if (return_code != NO_ERROR)
    {
        printf("[Error] Failed to add partition\n");
        return;
    }
    printf("[Success] Partition created with ID: %d\n", first_partition_id);

    test_create_process1(first_partition_id);
	add_new_partition(&second_partition_id, &return_code);
    if (return_code != NO_ERROR)
    {
        printf("[Error] Failed to add partition\n");
        return;
    }
    test_create_process2(second_partition_id);
    printf("[Success] Partition created with ID: %d\n", second_partition_id);

    printf("[Info] Starting partition scheduling...\n");
}

int32_t app_main(void)
{
	test_mos_and_partitions_creation();
	return 0;
}
