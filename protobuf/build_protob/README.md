使用第三方库生成protobuf的源文件, 版本是nanopb-0.3.9

目录说明:
nanopb-0.3.9-linux-x86: nanopb源码包
  pb.h             头文件
  pb_common.c/.h   接口定义
  pb_decode.c/.h   解码接口
  pb_encode.c/.h   编码接口
以上这些文件是protobuf主要文件,使用的时候必须包含这些头文件
  
install: 生成源文件的目录在install/protob/
  install/pb.h 在源文件基础上增加了#define PB_FIELD_32BIT 1
               support for tag numbers > 65536 and fields larger than 65536 bytes
  install/pb_* 与nanopb-0.3.9-linux-x86的文件相同

protob: 自定义消息的.proto文件及其.options文件,
        如果要增加一个消息,只要增加一个.proto文件和对应的.options文件即可

编译：
在ubuntu 14.04 32位下, 执行make命令, 生成的文件在install/protob/目录下
