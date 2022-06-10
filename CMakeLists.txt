cmake_minimum_required(VERSION 3.16) 

# set the project name 
project(LSSS) 

# specify the C++ standard 
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED True) 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_BUILD_TYPE Debug)

aux_source_directory(src SOURCES)
# include_directories(/usr/local/include/pbc)
# include_directories(/usr/include/eigen3)

# add the executable 
add_executable(main ${SOURCES}) 
target_include_directories(main PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
    /usr/local/include/pbc
    /usr/include/eigen3
)
link_libraries(/usr/local/lib)
target_link_libraries(main gmp)
target_link_libraries(main pbc)