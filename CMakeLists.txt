cmake_minimum_required(VERSION 3.23)
project(LAB1)

set(CMAKE_CXX_STANDARD 23)

# Add a compile definition for TRACY_ENABLE
#add_compile_definitions(TRACY_ENABLE)
# Add your source files
add_executable(LAB1 main.cpp TracyClient.cpp)
target_link_libraries(LAB1 PRIVATE ws2_32)
target_link_libraries(LAB1 PRIVATE dbghelp)