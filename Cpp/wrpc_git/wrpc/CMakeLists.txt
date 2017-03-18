Cmake
cmake_minimum_required(VERSION 3.7)

#项目信息
project (rb)
# 头文件搜索路径 gcc -I
include_directories(${rb_source_dir}/home/spike/WwIT_g/manager/internal_inc/)
#
# 查找当前目录下的所有源文件 并将名称保存到DIR_SRCS 变量
# aux_source_directory(. DIR_SRCS)
# add_executable(modeA ${DIR_SRCS})

#指定生成目标
add_executable(rb test.cc)

# 动态链接库或者静态链接库路径 gcc -L
link_directories(${rb_binary_dir}/usr/lib/,${rb_binary_dir}/usr/local/lib/)
# 添加链接库
target_link_libraries(rb libpthread.so,libSimpleAmqpClient.so)
