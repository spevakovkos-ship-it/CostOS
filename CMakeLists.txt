cmake_minimum_required(VERSION 3.15)
project(costos)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_executable(costos pkg/fm20/fm20.cpp modules/CostOSScript/CostOSScript.cpp pkg/counter/counter.cpp pkg/colorful_console/colorful_console.cpp modules/math_module.cpp modules/Errors_Table.cpp CostOSBoot/OSBoot.cpp)
add_compile_options(-fpermissive)
add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=1)
install(TARGETS costos DESTINATION bin)
