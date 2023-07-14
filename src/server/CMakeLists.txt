
# 定义变量，包含源文件
aux_source_directory(. SRC_LIST)

add_executable(ChatServer ${SRC_LIST})

target_compile_definitions(ChatServer muduo_net muduo_base pthread)