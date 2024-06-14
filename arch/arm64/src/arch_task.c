#include <arch_task.h>
#include <task.h>
#include <cpu.h>

extern void task_entry_point(task_id_t task_id);
extern void arch_exc_exit();

void arch_task_init(task_id_t task_id) {
	struct arch_esf_context *esf = NULL;
	struct task *task = ID_TO_TASK(task_id);

	/* root任务主动切换调度 */
	task->stack_ptr -= sizeof(struct arch_esf_context);
	esf = (struct arch_esf_context *)task->stack_ptr;
	esf->x0 = (uint64_t)task_id;
	esf->x1 = (uint64_t)task_entry_point;
	esf->spsr = SPSR_MODE_EL1H | DAIF_FIQ_BIT;
	esf->elr = (uint64_t)task_entry_point;

	/* 其它任务，在任务切换后，通过异常退出流程进入 */
	task->task_context.callee_context.sp_elx = (uint64_t)esf;
	task->task_context.callee_context.lr = (uint64_t)arch_exc_exit;
}
