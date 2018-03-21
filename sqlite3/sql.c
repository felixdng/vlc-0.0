/*
* date: 2018/1/23
* file: sql.c
* author: dengzhiqin
* describe: Android TV/Windows端数据库，用于保存配置信息
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#include "sql.h"

/* sqlite语句的最大长度 */
#define SQL_LENGTH 1024

static const char *TABLES[] = {
	/* 固定指令系统动作表 T_Action */
	"create table if not exists T_Action( \
		_id integer primary key autoincrement, \
		action_id text, \
		action_name text, \
		execute_order integer, \
		execute_time integer, \
		arguments text)"
};

#define DB_FILE "config.db"
static sqlite3 *db = NULL;

/*
* @func: 数据库初始化,包括打开数据库和建表,如果表已经存在,不会改变表原来的内容
*        注意:退出时要调用sql_exit关闭数据库
* @return: return zero on Success, others on failure.
*/
int sql_init(void)
{
	char *errmsg = NULL;
	int i;

	/* open database */
	if (SQLITE_OK != sqlite3_open(DB_FILE, &db)) {
		fprintf(stderr, "sqlite open failed\n");
		return -1;
	}

	/* create tables */
	for (i = 0; i < SQL_ARR_SIZE(TABLES); ++i) {
		if (SQLITE_OK != sqlite3_exec(db, TABLES[i], NULL, NULL, &errmsg)) {
			fprintf(stderr, "sqlite create tables failed : %s\n", errmsg);
			sqlite3_close(db);
			return -1;
		}
	}

	return 0;
}

/*
* @func: 退出关闭数据库,程序退出时调用
* @return: return void.
*/
void sql_exit(void)
{
	sqlite3_close(db);
}

/*
* @func: T_Action插入
* @param: action_id [in] 动作id
* @param: action_name [in] 动作名称,用于说明动作具体功能
* @param: execute_order [in] 十六进制指令
* @param: execute_time [in] 接收到指令后,动作执行间隔时间,单位毫秒
* @param: arguments [in] 动作参数,采用"key1:val1,key2:val2,...keyn:valn"格式存取
* @return: return zero on Success, others on failure.
*/
int t_action_insert(const char *action_id, const char *action_name, 
		long long execute_order, long long execute_time, const char *arguments)
{
	char *errmsg;
	char sql[SQL_LENGTH];
	snprintf(sql, sizeof(sql), 
		"insert into T_Action(action_id,action_name,execute_order,execute_time,arguments) values('%s','%s',%lld,%lld,'%s')", 
		action_id, action_name, execute_order, execute_time, arguments);

	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, &errmsg)) {
		fprintf(stderr, "sqlite exec failed : %s\n", errmsg);
		return -1;
	}

	return 0;
}

/*
* @func: T_Action删除
* @param: action_id [in] 动作id,删除表中与这个id对应的一行记录
* @return: return zero on Success, others on failure.
*/
int t_action_delete(const char *action_id)
{
	char *errmsg;
	char sql[SQL_LENGTH];
	snprintf(sql, sizeof(sql), "delete from T_Action where action_id='%s'", action_id);

	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, &errmsg)) {
		fprintf(stderr, "sqlite exec failed : %s\n", errmsg);
		return -1;
	}

	return 0;
}

/*
* @func: T_Action更新
* @param: action_id [in] 动作id,更新表中与这个id对应的一行记录
* @param: action_name [in] 动作名称,用于说明动作具体功能
* @param: execute_order [in] 十六进制指令
* @param: execute_time [in] 接收到指令后,动作执行间隔时间,单位毫秒
* @param: arguments [in] 动作参数,采用"key1:val1,key2:val2,...keyn:valn"格式存取
* @return: return zero on Success, others on failure.
*/
int t_action_update(const char *action_id, const char *action_name, 
	long long execute_order, long long execute_time, const char *arguments)
{
	char *errmsg;
	char sql[SQL_LENGTH];
	snprintf(sql, sizeof(sql), 
		"update T_Action set action_name='%s',execute_order=%lld,execute_time=%lld,arguments='%s' where action_id='%s'", 
		action_name, execute_order, execute_time, arguments, action_id);

	if (SQLITE_OK != sqlite3_exec(db, sql, NULL, NULL, &errmsg)) {
		fprintf(stderr, "sqlite exec failed : %s\n", errmsg);
		return -1;
	}

	return 0;
}

/*
* @func: T_Action分页查询所有动作
* @param: actions [out] 指令动作表数组,用于存储查询记录
* @param: size [in] 表示actions可以存储几行记录,即actions[]数组的最大个数
* @param: page_size [in] 每一页的大小
* @param: page_num [in] 页号,从1开始
* @return: 如果成功则返回实际查询的行数, 如果失败则返回-1
*/
int t_action_select_page(t_action *actions, int size, int page_size, int page_num)
{
	char **result = NULL;
	char *errmsg;
	int ret, nrow, ncolumn, i, count;
	char sql[SQL_LENGTH];
	snprintf(sql, sizeof(sql), 
		"select action_id,action_name,execute_order,execute_time,arguments from T_Action order by action_id limit (%d-1)*%d,%d", 
		page_num, page_size, page_size);

	ret = sqlite3_get_table(db, sql, &result, &nrow, &ncolumn, &errmsg);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "sqlite3_get_table failed : %s\n", errmsg);
		goto err_exit;
	}

	/* 没有查询到任何数据 */
	if (nrow <= 0)
		goto err_exit;

	/* 最大行数不能大于size */
	count = nrow < size ? nrow : size;
	for (i = 0; i < count; ++i) {
		strncpy(actions[i].action_id, result[(i + 1)*ncolumn], sizeof(actions[i].action_id));
		strncpy(actions[i].action_name, result[(i + 1)*ncolumn + 1], sizeof(actions[i].action_name));
		actions[i].execute_order = atoll(result[(i + 1)*ncolumn + 2]);
		actions[i].execute_time = atoll(result[(i + 1)*ncolumn + 3]);
		strncpy(actions[i].arguments, result[(i + 1)*ncolumn + 4], sizeof(actions[i].arguments));
	}

	sqlite3_free_table(result);
	return count;

err_exit:
	sqlite3_free_table(result);
	return -1;
}

/*
* @func: T_Action查询所有指令类型
* @param: orders [out] 指令数组,用于存储查询记录
* @param: size [in] 表示orders可以存储几行记录,即orders[]数组的最大个数
* @return: 如果成功则返回实际查询的行数, 如果失败则返回-1
*/
int t_action_select_orders(long long *orders, int size)
{
	char **result = NULL;
	char *errmsg;
	int ret, nrow, ncolumn, i, count;
	char sql[] = "select execute_order from T_Action order by execute_order";

	ret = sqlite3_get_table(db, sql, &result, &nrow, &ncolumn, &errmsg);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "sqlite3_get_table failed : %s\n", errmsg);
		goto err_exit;
	}

	/* 没有查询到任何数据 */
	if (nrow <= 0)
		goto err_exit;

	/* 最大行数不能大于size */
	count = nrow < size ? nrow : size;
	for (i = 0; i < count; ++i) {
		orders[i] = atoll(result[(i + 1)*ncolumn]);
	}

	sqlite3_free_table(result);
	return count;

err_exit:
	sqlite3_free_table(result);
	return -1;
}

/*
* @func: T_Action按指令类型查询所有动作
* @param: actions [out] 指令动作表数组,用于存储查询记录
* @param: size [in] 表示actions可以存储几行记录,即actions[]数组的最大个数
* @param: order [in] 指令类型,查询关键字
* @return: 如果成功则返回实际查询的行数, 如果失败则返回-1
*/
int t_action_select_by_order(t_action *actions, int size, long long order)
{
	char **result = NULL;
	char *errmsg;
	int ret, nrow, ncolumn, i, count;
	char sql[SQL_LENGTH];
	snprintf(sql, sizeof(sql), 
		"select action_id,action_name,execute_order,execute_time,arguments from T_Action where execute_order=%lld order by action_id", 
		order);

	ret = sqlite3_get_table(db, sql, &result, &nrow, &ncolumn, &errmsg);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "sqlite3_get_table failed : %s\n", errmsg);
		goto err_exit;
	}

	/* 没有查询到任何数据 */
	if (nrow <= 0)
		goto err_exit;

	/* 最大行数不能大于size */
	count = nrow < size ? nrow : size;
	for (i = 0; i < count; ++i) {
		strncpy(actions[i].action_id, result[(i + 1)*ncolumn], sizeof(actions[i].action_id));
		strncpy(actions[i].action_name, result[(i + 1)*ncolumn + 1], sizeof(actions[i].action_name));
		actions[i].execute_order = atoll(result[(i + 1)*ncolumn + 2]);
		actions[i].execute_time = atoll(result[(i + 1)*ncolumn + 3]);
		strncpy(actions[i].arguments, result[(i + 1)*ncolumn + 4], sizeof(actions[i].arguments));
	}

	sqlite3_free_table(result);
	return count;

err_exit:
	sqlite3_free_table(result);
	return -1;
}

/*
* @func: T_Action查询所有动作名称
* @param: names [out] 动作名称数组,用于存储查询记录
* @param: size [in] 表示names可以存储几行记录,即names[]数组的最大个数
* @param: len [in] 表示每一个动作名称的最大长度
* @return: 如果成功则返回实际查询的行数, 如果失败则返回-1
*/
int t_action_select_names(char *names, int size, int len)
{
	char **result = NULL;
	char *errmsg;
	int ret, nrow, ncolumn, i, count;
	char sql[] = "select action_name from T_Action order by action_name";

	ret = sqlite3_get_table(db, sql, &result, &nrow, &ncolumn, &errmsg);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "sqlite3_get_table failed : %s\n", errmsg);
		goto err_exit;
	}

	/* 没有查询到任何数据 */
	if (nrow <= 0)
		goto err_exit;

	/* 最大行数不能大于size */
	count = nrow < size ? nrow : size;
	for (i = 0; i < count; ++i) {
		strncpy(names + i * len, result[(i + 1)*ncolumn], len);
	}

	sqlite3_free_table(result);
	return count;

err_exit:
	sqlite3_free_table(result);
	return -1;
}

/*
* @func: T_Action按动作名称查询指定动作
* @param: actions [out] 指令动作表数组,用于存储查询记录
* @param: size [in] 表示actions可以存储几行记录,即actions[]数组的最大个数
* @param: name [in] 动作名称,查询关键字
* @return: 如果成功则返回实际查询的行数, 如果失败则返回-1
*/
int t_action_select_by_name(t_action *actions, int size, const char *name)
{
	char **result = NULL;
	char *errmsg;
	int ret, nrow, ncolumn, i, count;
	char sql[SQL_LENGTH];
	snprintf(sql, sizeof(sql),
		"select action_id,action_name,execute_order,execute_time,arguments from T_Action where action_name='%s' order by action_id",
		name);

	ret = sqlite3_get_table(db, sql, &result, &nrow, &ncolumn, &errmsg);
	if (SQLITE_OK != ret) {
		fprintf(stderr, "sqlite3_get_table failed : %s\n", errmsg);
		goto err_exit;
	}

	/* 没有查询到任何数据 */
	if (nrow <= 0)
		goto err_exit;

	/* 最大行数不能大于size */
	count = nrow < size ? nrow : size;
	for (i = 0; i < count; ++i) {
		strncpy(actions[i].action_id, result[(i + 1)*ncolumn], sizeof(actions[i].action_id));
		strncpy(actions[i].action_name, result[(i + 1)*ncolumn + 1], sizeof(actions[i].action_name));
		actions[i].execute_order = atoll(result[(i + 1)*ncolumn + 2]);
		actions[i].execute_time = atoll(result[(i + 1)*ncolumn + 3]);
		strncpy(actions[i].arguments, result[(i + 1)*ncolumn + 4], sizeof(actions[i].arguments));
	}

	sqlite3_free_table(result);
	return count;

err_exit:
	sqlite3_free_table(result);
	return -1;
}
