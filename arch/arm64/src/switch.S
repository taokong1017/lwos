#include <cpu.h>
#include <offsets.h>
#include <task_offsets.h>
#include <menuconfig.h>
#include <general.h>

/*
 * void arch_task_context_switch(arch_task_context_t *new, arch_task_context_t *old)
 */
.globl arch_task_context_switch
.type arch_task_context_switch, function
.section .text, "ax"
arch_task_context_switch:
	/* save cpu registers */
	str x19, [x1, #ARCH_TASK_CONTEXT_X19_OFFSET]
	stp x20, x21, [x1, #ARCH_TASK_CONTEXT_X20_X21_OFFSET]
	stp x22, x23, [x1, #ARCH_TASK_CONTEXT_X22_X23_OFFSET]
	stp x24, x25, [x1, #ARCH_TASK_CONTEXT_X24_X25_OFFSET]
	stp x26, x27, [x1, #ARCH_TASK_CONTEXT_X26_X27_OFFSET]
	stp x28, x29, [x1, #ARCH_TASK_CONTEXT_X28_X29_OFFSET]
	mrs x2, daif
	mrs x3, nzcv
	stp x2, x3,  [x1, #ARCH_TASK_CONTEXT_DAIF_NZCV_OFFSET]
	mov x2, sp
	stp x30, x2, [x1, #ARCH_TASK_CONTEXT_X30_SP_OFFSET]

#ifdef CONFIG_FPU_ENABLE
	stp x0, x1, [sp, #-16]!
	add x0, x1, #ARCH_TASK_FP_CONTEXT_OFFSET
	bl arch_fpu_save
	ldp x0, x1, [sp], #16
	stp x0, x1, [sp, #-16]!
	add x0, x0, #ARCH_TASK_FP_CONTEXT_OFFSET
	bl arch_fpu_restore
	ldp x0, x1, [sp], #16
#endif
	/* switch ttbr0_el1 */
	sub x1, x0, #TASK_OF_ARCH_TASK_CONTEXT_OFFSET
	add x19, x1, #TASK_OF_MEM_DOMAIN_OFFSET
	ldr x19, [x19]
	add x19, x19, #MEM_DOMAIN_OF_ARCH_MEM_DOMAIN_OFFSET
	add x19, x19, #ARCH_MEM_DOMAIN_OF_PGTABLE_OFFSET
	add x19, x19, #MMU_PGTABLE_OF_TTBR0_OFFSET
	ldr x19, [x19]
	msr ttbr0_el1, x19
	isb

	/* switch task stack */
	add x1, x1, #TASK_OF_FLAG_OFFSET
	ldr x1, [x1]
	and x1, x1, #0x4 /* TASK_FLAG_USER */
	cmp x1, #0
	mov x1, #0x1
	csel x1, x1, xzr, eq
	msr spsel, x1

	/* restore cpu registers */
	ldp x30, x2,  [x0, #ARCH_TASK_CONTEXT_X30_SP_OFFSET]
	mov sp, x2
	ldp x2, x3,   [x0, #ARCH_TASK_CONTEXT_DAIF_NZCV_OFFSET]
	msr daif, x2
	msr nzcv, x3
	ldp x28, x29, [x0, #ARCH_TASK_CONTEXT_X28_X29_OFFSET]
	ldp x26, x27, [x0, #ARCH_TASK_CONTEXT_X26_X27_OFFSET]
	ldp x24, x25, [x0, #ARCH_TASK_CONTEXT_X24_X25_OFFSET]
	ldp x22, x23, [x0, #ARCH_TASK_CONTEXT_X22_X23_OFFSET]
	ldp x20, x21, [x0, #ARCH_TASK_CONTEXT_X20_X21_OFFSET]
	ldr x19, [x0, #ARCH_TASK_CONTEXT_X19_OFFSET]
	ret

/*
 * void arch_main_task_switch(struct task *task)
 */
.globl arch_main_task_switch
.type arch_main_task_switch, function
.section .text, "ax"
arch_main_task_switch:
	/* switch to new task */
	mov w1, 0x10 /* TASK_STATUS_RUNNING */
	str w1, [x0, TASK_OF_STATUS_OFFSET]
	add x1, x0, TASK_OF_ARCH_TASK_CONTEXT_OFFSET
	add x1, x1, TASK_CONTEXT_OF_CALLEE_CONTEXT_OFFSET
	ldp x2, x3, [x1, CALLEE_CONTEXT_OF_X30_SP_OFFSET]
	add x1, x1, #TASK_OF_FLAG_OFFSET
	ldr x1, [x1]
	and x1, x1, #0x4 /* TASK_FLAG_USER */
	cmp x1, #0
	mov x1, #0x1
	csel x1, x1, xzr, eq
	msr spsel, x1
	mov sp, x3
	blr x2
	ret

/*
 * void arch_exc_exit()
 */
.globl arch_exc_exit
.type arch_exc_exit, function
.section .text, "ax"
arch_exc_exit:
	msr daifset, (DAIFSET_IRQ_BIT | DAIFSET_FIQ_BIT)
	ldp x0, x1, [sp, #ESF_CONTEXT_OF_SPSR_ELR_OFFSET]
	msr spsr_el1, x0
	msr elr_el1, x1
	ldr x0, [sp, #ESF_CONTEXT_OF_X0_X1_OFFSET]
	add x0, x0, #TASK_OF_MEM_DOMAIN_OFFSET
	ldr x0, [x0]
	add x0, x0, #MEM_DOMAIN_OF_ARCH_MEM_DOMAIN_OFFSET
	add x0, x0, #ARCH_MEM_DOMAIN_OF_PGTABLE_OFFSET
	add x0, x0, #MMU_PGTABLE_OF_TTBR0_OFFSET
	ldr x0, [x0]
	msr ttbr0_el1, x0
	isb
	ldp x0, x1, [sp, #ESF_CONTEXT_OF_X0_X1_OFFSET]
	ldp x2, x3, [sp, #ESF_CONTEXT_OF_X2_X3_OFFSET]
	ldp x4, x5, [sp, #ESF_CONTEXT_OF_X4_X5_OFFSET]
	ldp x6, x7, [sp, #ESF_CONTEXT_OF_X6_X7_OFFSET]
	ldp x8, x9, [sp, #ESF_CONTEXT_OF_X8_X9_OFFSET]
	ldp x10, x11, [sp, #ESF_CONTEXT_OF_X10_X11_OFFSET]
	ldp x12, x13, [sp, #ESF_CONTEXT_OF_X12_X13_OFFSET]
	ldp x14, x15, [sp, #ESF_CONTEXT_OF_X14_X15_OFFSET]
	ldp x16, x17, [sp, #ESF_CONTEXT_OF_X16_X17_OFFSET]
	ldp x18, lr,  [sp, #ESF_CONTEXT_OF_X18_LR_OFFSET]
	add sp, sp, #ESF_CONTEXT_SIZE
	eret

/*
 * uint64_t current_pc_get()
 */
.globl current_pc_get
.type current_pc_get, function
.section .text, "ax"
current_pc_get:
	mov x0, lr
	ret

/*
 * uint64_t current_sp_get()
 */
.globl current_sp_get
.type current_sp_get, function
.section .text, "ax"
current_sp_get:
	mov x0, sp
	ret
