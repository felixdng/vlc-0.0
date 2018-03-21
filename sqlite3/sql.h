/*
* date: 2018/1/23
* file: sql.h
* author: dengzhiqin
* describe: sql.c的头文件
*/
#ifndef __SQL_H
#define __SQL_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _t_action {
	char action_id[128];      /* 动作id */
	char action_name[64];     /* 动作名称,用于说明动作具体功能 */
	long long execute_order;  /* 动作指令,用64位十六进制数表示 */
	long long execute_time;   /* 接收到指令后,动作执行间隔时间,单位毫秒 */
	char arguments[256];      /* 动作参数 */
}t_action;

/* 计算数组的元素个数 */
#define SQL_ARR_SIZE(_arr) \
	(sizeof(_arr) / sizeof(_arr[0]))

/*
* @func: 数据库初始化,包括打开数据库和建表,如果表已经存在,不会改变表原来的内容
*        注意:退出时要调用sql_exit关闭数据库
* @return: return zero on Success, others on failure.
*/
int sql_init(void);

/*
* @func: 退出关闭数据库,程序退出时调用
* @return: return void.
*/
void sql_exit(void);

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
	long long execute_order, long long execute_time, const char *arguments);

/*
* @func: T_Action删除
* @param: action_id [in] 动作id,删除表中与这个id对应的一行记录
* @return: return zero on Success, others on failure.
*/
int t_action_delete(const char *action_id);

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
	long long execute_order, long long execute_time, const char *arguments);

/*
* @func: T_Action分页查询所有动作
* @param: actions [out] 指令动作表数组,用于存储查询记录
* @param: size [in] 表示actions可以存储几行记录,即actions[]数组的最大个数
* @param: page_size [in] 每一页的大小
* @param: page_num [in] 页号,从1开始
* @return: 如果成功则返回实际查询的行数, 如果失败则返回-1
*/
int t_action_select_page(t_action *actions, int size, int page_size, int page_num);

/*
* @func: T_Action查询所有指令类型
* @param: orders [out] 指令数组,用于存储查询记录
* @param: size [in] 表示orders可以存储几行记录,即orders[]数组的最大个数
* @return: 如果成功则返回实际查询的行数, 如果失败则返回-1
*/
int t_action_select_orders(long long *orders, int size);

/*
* @func: T_Action按指令类型查询所有动作
* @param: actions [out] 指令动作表数组,用于存储查询记录
* @param: size [in] 表示actions可以存储几行记录,即actions[]数组的最大个数
* @param: order [in] 指令类型,查询关键字
* @return: 如果成功则返回实际查询的行数, 如果失败则返回-1
*/
int t_action_select_by_order(t_action *actions, int size, long long order);

/*
* @func: T_Action查询所有动作名称
* @param: names [out] 动作名称数组,用于存储查询记录
* @param: size [in] 表示names可以存储几行记录,即names[]数组的最大个数
* @param: len [in] 表示每一个动作名称的最大长度
* @return: 如果成功则返回实际查询的行数, 如果失败则返回-1
*/
int t_action_select_names(char *names, int size, int len);

/*
* @func: T_Action按动作名称查询指定动作
* @param: actions [out] 指令动作表数组,用于存储查询记录
* @param: size [in] 表示actions可以存储几行记录,即actions[]数组的最大个数
* @param: name [in] 动作名称,查询关键字
* @return: 如果成功则返回实际查询的行数, 如果失败则返回-1
*/
int t_action_select_by_name(t_action *actions, int size, const char *name);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
