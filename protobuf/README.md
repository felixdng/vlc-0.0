1. 文件功能说明
src/protocol.c/.h : 自定义协议接口
src/protob.c/.h : 实现所有消息类型的protobuf编解码
src/msg_handler.c/.h : 消息自动处理机制,这里使用数组实现消息查询,
       也可以用哈希表或链表实现,在这里改变实现方式不会对其他文件造成影响
src/util.c/.h : 通用工具,主要是整型和字节码的转换,以及校验码的计算

protocoltest.c : 示例代码

protobuf/ : protobuf消息协议源码

TCPService : 服务器端示例代码
TCPClient: 客户端示例代码

2. 使用说明
主要接口:
1. void protob_init(void)
    在protob.h,使用之前必须先调用此接口,protobuf的初始化
2. protocol.h
  cli_to_serv_encode : 对客户端发往服务器的消息编码,注意要用free()释放返回值
  cli_to_serv_decode : 对客户端发往服务器的消息解码,注意要用cmsg_free()释放返回值
  serv_to_cli_encode : 对服务器发往客户端的消息编码,注意要用free()释放返回值
  serv_to_cli_decode : 对服务器发往客户端的消息解码,注意要用smsg_free()释放返回值
  cmsg_free() : 释放内存,与cli_to_serv_decode配合使用
  smsg_free() : 释放内存,与serv_to_cli_decode配合使用

3. build_protob
用于生成自定义protobuf消息编解码的源文件
