 # Automatically generated CMakeLists.txt

set (target libm3)
set (sources ./log.cpp
./m3header.h
./quaternion.h
./vec3d.h
./model.h
./model.cpp
./log.hpp)

add_library(${target} ${sources})
target_include_directories(${target} PUBLIC .)
target_link_libraries(${target} Boost::filesystem)
