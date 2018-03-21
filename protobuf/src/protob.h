#ifndef __PROTOB_H
#define __PROTOB_H

#include "Connect.pb.h"
#include "Disconnect.pb.h"
#include "AddAction.pb.h"
#include "ModifyAction.pb.h"
#include "DeleteAction.pb.h"
#include "SearchActionByPage.pb.h"
#include "GetOrders.pb.h"
#include "SearchActionByOrder.pb.h"
#include "GetActionNames.pb.h"
#include "SearchActionByName.pb.h"
#include "Upload.pb.h"
#include "Download.pb.h"
#include "Activation.pb.h"

#ifdef __cplusplus
extern "C" {
#endif

/* protobuf初始化 */
void protob_init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
