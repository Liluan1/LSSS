cmake_minimum_required(VERSION 3.16) 

# set the project name 
project(LSSS) 

# specify the C++ standard 
set(CMAKE_CXX_STANDARD 11) 
set(CMAKE_CXX_STANDARD_REQUIRED True) 
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_BUILD_TYPE Debug)

aux_source_directory(src SOURCES)

# add the executable 
add_executable(main ${SOURCES}) 
target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(main PRIVATE /usr/local/include/pbc)
target_include_directories(main PRIVATE /usr/include/eigen3)
target_link_libraries(main gmp)
target_link_libraries(main pbc)