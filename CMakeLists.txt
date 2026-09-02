cmake_minimum_required(VERSION 3.15)
project(costos)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(costos pkg/fm20/fm20.cpp  pkg/counter/counter.cpp pkg/colorful_console/colorful_console.cpp modules/math_module.cpp modules/Errors_Table.cpp CostOSBoot/OSBoot.cpp)

install(TARGETS costos DESTINATION bin)