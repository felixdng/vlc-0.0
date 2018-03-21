/*
* date: 2018/1/23
* file: sqltest.c
* author: dengzhiqin
* describe: 测试程序
*/
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include "sql.h"

static void show_actions(const t_action *t, int count)
{
	int i;
	for (i = 0; i < count; i++) {
		printf("action_id: %s\n", t[i].action_id);
		printf("action_name: %s\n", t[i].action_name);
		printf("execute_order: 0x%llx\n", t[i].execute_order);
		printf("execute_time: %lld\n", t[i].execute_time);
		printf("arguments: %s\n", t[i].arguments);
		printf("=======================================================\n\n");
	}
}

static unsigned int args_flag = 0;

static void usage_info(const char *app)
{
	printf("=====================================\n");
	printf("Usage for %s:\n", app);
	printf("  %s [options]\n", app);
	printf("\n");
	printf("[options]\n");
	printf("  --insert        : insert into tabel\n");
	printf("  --delete        : delete from tabel\n");
	printf("  --update        : update into tabel\n");
	printf("  --select_page   : select from tabel by page\n");
	printf("  --select_orders : select orders from tabel\n");
	printf("  --by_order      : select from tabel by order\n");
	printf("  --select_names  : select names from tabel\n");
	printf("  --by_name       : select from tabel by name\n");
	printf("  -h              : help infomation\n");
	printf("=====================================\n");
}

static struct option opts[] = {
	{"insert", 0, NULL, 1},
	{"delete", 0, NULL, 2},
	{"update", 0, NULL, 3},
	{"select_page", 0, NULL, 4},
	{"select_orders", 0, NULL, 5},
	{"by_order", 0, NULL, 6},
	{"select_names", 0, NULL, 7},
	{"by_name", 0, NULL, 8},
	{0,0,0,0}
};

static int args_handler(int argc, char **argv)
{
	int opt = 0;
	while ((opt = getopt_long(argc, argv, "h", opts, NULL)) != -1) {
		switch (opt) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			args_flag |= 1 << opt;
			break;
		case 'h':
		default:
			usage_info(argv[0]);
			return 1;
		}
	}
	return 0;
}

int main(int argc, char **argv)
{
	int count, i;
	t_action t1[10];


	if (argc < 2) {
		usage_info(argv[0]);
		return 1;
	}
	if (args_handler(argc, argv))
		return 1;

	//初始化
	sql_init();


	if (args_flag & (1 << 1)) {
		printf(">>> insert\n");
		t_action_insert("1234-0", "play", 0xabcd000000000001, 4, "key:val");
		t_action_insert("1234-1", "pause", 0xabcd000000000002, 4, "key:val2");
		t_action_insert("1234-2", "speed", 0xabcd000000000004, 4, "key:val2");
		t_action_insert("1234-3", "rewind", 0xabcd000000000008, 4, "key:val2");
		t_action_insert("1234-4", "last", 0xabcd000000000010, 4, "key:val2");
		t_action_insert("1234-5", "next", 0xabcd000000000020, 4, "key:val2");
		t_action_insert("1234-6", "stop", 0xabcd000000000080, 4, "key:val3");
		t_action_insert("1234-7", "vol+", 0xabcd000000002000, 4, "key:val3");
		t_action_insert("1234-8", "vol-", 0xabcd000000001000, 4, "key:val3");
	}

	if (args_flag & (1 << 2)) {
		printf("> delete\n");
		t_action_delete("1234-7");
	}

	if (args_flag & (1 << 3)) {
		printf("> update\n");
		t_action_update("1234-5", "next", 0xabcd000000000020, 8, "key2:val2,key1:val1");
	}


	if (args_flag & (1 << 4)) {
		printf("> select_page\n");
		count = t_action_select_page(t1, SQL_ARR_SIZE(t1), 10, 1);
		printf("select 结果: %d\n", count);
		show_actions(t1, count);
	}

	if (args_flag & (1 << 5)) {
		printf("> select_orders\n");
		long long orders[10];
		count = t_action_select_orders(orders, SQL_ARR_SIZE(orders));
		printf("查询指令 结果: %d\n", count);
		for (i = 0; i < count; i++)
			printf("0x%llx\n", orders[i]);
		printf("\n");
	}

	if (args_flag & (1 << 6)) {
		printf("> by_order\n");
		count = t_action_select_by_order(t1, SQL_ARR_SIZE(t1), 0xabcd000000000080);
		printf("select 结果: %d\n", count);
		show_actions(t1, count);
	}

	if (args_flag & (1 << 7)) {
		printf("> select_names\n");
		char names[10][20];
		count = t_action_select_names((char *)names, 10, 20);
		for (i = 0; i < count; i++)
			printf("%s\n", names[i]);
		printf("\n");
	}

	if (args_flag & (1 << 8)) {
		printf("> by_name\n");
		count = t_action_select_by_name(t1, SQL_ARR_SIZE(t1), "play");
		printf("select 结果: %d\n", count);
		show_actions(t1, count);
	}

	sql_exit();
	while (1);
	return 0;
}
