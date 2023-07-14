
# 定义SRC_LIST变量，包含该目录下源文件
aux_source_directory(. SRC_LIST)

# 指定可执行文件
add_executable(CharServer ${SRC_LIST})

target_link_libraries(CharServer muduo_base muduo_net pthread)