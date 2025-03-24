#include <ucx.h>

void test_create_process() {
    process_attribute_type attributes;
    process_id_type process_id;
    return_code_type return_code;
    attributes.stack_size = 1024;
    attributes.base_priority = 10;
    attributes.period = 1000;
    attributes.time_capacity = 500;
    create_process(&attributes, &process_id, &return_code);
    printf("Test 1 : return_code = %d\n", return_code);
}

void test_task1(void)
{
    printf("Executing test task in partition 1\n");
}

void test_task2(void)
{
    printf("Executing test task in partition 2\n");
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
    
	system_address_type fisrt_app = (void*)test_task1;
	add_new_partition(fisrt_app,&first_partition_id, &return_code);
    if (return_code != NO_ERROR)
    {
        printf("[Error] Failed to add partition\n");
        return;
    }
    printf("[Success] Partition created with ID: %d\n", first_partition_id);

    system_address_type second_app = (void*)test_task2;
	add_new_partition(second_app,&second_partition_id, &return_code);
    if (return_code != NO_ERROR)
    {
        printf("[Error] Failed to add partition\n");
        return;
    }
    printf("[Success] Partition created with ID: %d\n", second_partition_id);

    printf("[Info] Starting partition scheduling...\n");
}

int32_t app_main(void)
{
	test_mos_and_partitions_creation();
	return 0;
}
