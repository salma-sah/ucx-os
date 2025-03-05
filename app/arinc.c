#include <ucx.h>
void test_mos_creation() {
	uint16_t mos_id;
	return_code_type return_code;
	partition_id_type first_partition_id;

    create_mos(&mos_id, &return_code);
    printf("MOS CREATED");
    
	system_address_type fisrt_app = (void*)test_create_process;
	add_new_partition(mos_id,fisrt_app,&first_partition_id, &return_code);
    printf("TEST CREATE PROCESS PARTITION CREATED");
}

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

int32_t app_main(void)
{
	test_create_process();
	return 0;
}
