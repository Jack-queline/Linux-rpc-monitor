#添加需要编译的源文件
set(SOURCES
   main.cpp
   monitor/cpu_softirq_monitor.cpp
   monitor/cpu_load_monitor.cpp
   monitor/cpu_stat_monitor.cpp
   monitor/mem_monitor.cpp
   monitor/net_monitor.cpp
   utils/read_file.cpp
)
#monitor为最终生成的可执行文件的名字
add_executable(monitor ${SOURCES})

#添加需要的头文件与依赖，同时指定好其通用的前缀地址，方便源文件对头文件的包含
target_include_directories(monitor PUBLIC
  ${PROJECT_SOURCE_DIR}/test_monitor/include
  ${PROJECT_SOURCE_DIR}/rpc_manager
)
#添加需要用到的库，client为grpc生成的库
target_link_libraries(monitor
    PUBLIC
    monitor_proto
    client
    boost_system
    boost_chrono
)