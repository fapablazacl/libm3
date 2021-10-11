 # Automatically generated CMakeLists.txt

set (target test)
set (sources ./main.cpp)

add_executable(${target} ${sources})
target_link_libraries(${target} libm3 Boost::filesystem)
