
project(libm3)

find_package(Boost COMPONENTS REQUIRED filesystem)

add_subdirectory(src/lib)
add_subdirectory(src/test)
