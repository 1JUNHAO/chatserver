 cmake_minimum_required(VERSION 3.0) ##要求使用的cmkae最低版本
 project(demo)  #定义工程project名字
 set(CMAKE_BUILD_TYPE "Debug") # 设置为debug模式
 set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)  #设置编译选项

 #配置头文件搜索路径
 # include_directories()
 # 配置库文件搜索路径
 # link_directories()


# 设置需要编译的源文件列表
 set(SRC_LIST muduo_server.cpp)


# 设置可执行文件最终存储位置
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#把.指定路径下所有的源文件名字放入变量SRC_LIST中
# aux_source_directory(. SRC_LIST)

# 表示生成可执行文件server,由src_list变量所定义的源文件编译而来
 add_executable(server ${SRC_LIST})

 # 表示server这个程序需要链接以下三个库文件
 target_link_libraries(server muduo_net muduo_base pthread)