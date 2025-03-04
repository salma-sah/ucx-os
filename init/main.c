/* file:          main.c
 * description:   UCX/OS kernel
 * date:          10/2022
 * author:        Sergio Johann Filho <sergio.johann@acad.pucrs.br>
 */

#include <ucx.h>

/* main() function, called from the C runtime */

int32_t main(void)
{
	struct tcb_s *task;
	int32_t pr;
	/*
	uint16_t mos_id;
	return_code_type return_code;
	partition_id_type first_partition_id;
	*/
	_hardware_init();

	printf("UCX/OS v%s boot on %s\n", __VER__, __ARCH__);
#ifndef UNKNOWN_HEAP
	ucx_heap_init((size_t *)&_heap_start, (size_t)&_heap_size);
	printf("heap_init(), %d bytes free\n", (size_t)&_heap_size);
#else
	ucx_heap_init((size_t *)&__bss_end, ((size_t)&__stack - (size_t)&__bss_end - 256));
	printf("heap_init(), %d bytes free\n", ((size_t)&__stack - (size_t)&__bss_end - 256));
#endif
	kcb->tasks = list_create();

	if (!kcb->tasks)
		krnl_panic(ERR_KCB_ALLOC);

	pr = app_main();
	setjmp(kcb->context);

	if (!kcb->tasks->length)
		krnl_panic(ERR_NO_TASKS);

	//TODO REVOIR REFERENCE
	/*create_mos(&mos_id, &return_code);

	system_address_type fisrt_app = (void*)test_create_process;
	add_new_partition(mos_id,fisrt_app,&first_partition_id, &return_code);
	*/

	kcb->preemptive = pr ? 'y' : 'n';
	kcb->task_current = kcb->tasks->head->next;
	task = kcb->task_current->data;
	_dispatch_init(task->context);

	/* never reached */
	return 0;
}
