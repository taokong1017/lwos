#include <compiler.h>
#include <user_space.h>

#define TASK_STACK_SIZE 0x2000
static struct mem_domain app_mem_domain;
APP_PARTITION_DEFINE(app1);

#define ROOT_TASK_NAME "Root_Task"
APP_DATA(app1) task_id_t root_task_id = -1;
APP_DATA(app1) struct task root_task = {0};
APP_BSS(app1) char root_stack[TASK_STACK_SIZE] = {0};

#define TASK_A_NAME "Task_A"
APP_DATA(app1) task_id_t task_a_id = -1;
APP_DATA(app1) struct task task_a = {0};
APP_BSS(app1) char task_a_stack[TASK_STACK_SIZE] = {0};

#define TASK_B_NAME "Task_B"
APP_DATA(app1) task_id_t task_b_id = -1;
APP_DATA(app1) struct task task_b = {0};
APP_BSS(app1) char task_b_stack[TASK_STACK_SIZE] = {0};

#define USER_SEM_NAME "User_Sem"
#define USER_SEM_MAX_CNT 1
#define USER_SEM_INIT_CNT 0
APP_DATA(app1) sem_id_t user_sem_id = -1;

static void task_a_entry(void *arg0, void *arg1, void *arg2, void *arg3) {
	(void)arg1;
	(void)arg2;
	(void)arg3;
	sem_id_t sem_id = (sem_id_t)arg0;

	forever() {
		user_sem_give(sem_id);
		user_task_delay(1);
	}
}

static void task_b_entry(void *arg0, void *arg1, void *arg2, void *arg3) {
	(void)arg1;
	(void)arg2;
	(void)arg3;
	sem_id_t sem_id = (sem_id_t)arg0;

	forever() { user_sem_take(sem_id, SEM_WAIT_FOREVER); }
}

static void user_task_entry(void *arg0, void *arg1, void *arg2, void *arg3) {
	(void)arg0;
	(void)arg0;
	(void)arg0;
	(void)arg0;

	/* Create user semphore */
	user_sem_create(USER_SEM_NAME, USER_SEM_INIT_CNT, USER_SEM_MAX_CNT,
					&user_sem_id);

	/* Create and start task A */
	user_task_create(&task_a_id, TASK_A_NAME, task_a_entry, (void *)user_sem_id, NULL,
					 NULL, NULL, &task_a, &task_a_stack, TASK_STACK_SIZE,
					 TASK_FLAG_USER | TASK_FLAG_INHERIT_PERM);
	user_task_priority_set(task_a_id, TASK_PRIORITY_HIGHEST - 2);
	user_task_start(task_a_id);

	/* Create and start task B */
	user_task_create(&task_b_id, TASK_B_NAME, task_b_entry, (void *)user_sem_id, NULL,
					 NULL, NULL, &task_b, &task_b_stack, TASK_STACK_SIZE,
					 TASK_FLAG_USER | TASK_FLAG_INHERIT_PERM);
	user_task_priority_set(task_b_id, TASK_PRIORITY_HIGHEST - 1);
	user_task_start(task_b_id);

	return;
}

int main() {
	app_mem_domain_init(&app_mem_domain, "app1", app1);
	mem_domain_set_up(&app_mem_domain);

	/* 创建用户态根任务 */
	task_create_with_stack(&root_task_id, ROOT_TASK_NAME, user_task_entry, NULL,
						   NULL, NULL, NULL, &root_task, &root_stack,
						   TASK_STACK_SIZE, TASK_FLAG_USER);
	task_mem_domain_add(root_task_id, &app_mem_domain);
	task_priority_set(root_task_id, TASK_PRIORITY_HIGHEST);
	task_start(root_task_id);

	return 0;
}