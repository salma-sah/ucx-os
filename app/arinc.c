#include <ucx.h>
#include <limits.h>

void test_create_process() {
    process_attribute_type attributes;
    process_id_type process_id;
    return_code_type return_code;

    // Valid parameters
    attributes.stack_size = 1024;
    attributes.base_priority = 10;
    attributes.period = 1000;
    attributes.time_capacity = 500;
    create_process(&attributes, &process_id, &return_code);
    printf("Test 1 : return_code = %d\n", return_code);

    // time_capacity greater than period
    attributes.base_priority = 10;
    attributes.time_capacity = 2000;
    create_process(&attributes, &process_id, &return_code);
    printf("Test 4 : return_code = %d\n", return_code);
}

int32_t app_main(void)
{
	test_create_process();
	return 0;
}
