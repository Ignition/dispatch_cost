cmake_minimum_required(VERSION 3.16)
project(dispach_cost)

set(BUILD_TESTING OFF)
set(BENCHMARK_ENABLE_TESTING OFF)
add_subdirectory(benchmark)
add_subdirectory(proxy)

add_executable(bench main.cpp)
target_link_libraries(bench PRIVATE benchmark::benchmark_main msft_proxy)
