cmake_minimum_required(VERSION 3.10)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the shared library.
add_library(
    ywen_cpp
    STATIC
    "./boost_ext/filesystem.cpp"
    "./file/file.cpp"
    "./tmpdir/tmpdir.cpp"
)

# ##################################################

# Set the project name.
project(bfs DESCRIPTION "Verify boost::filesystem behaviors")

# Add the executable.
add_executable(
    bfs
    "./programs/bfs.cpp"
)

# Add the include and library directories.
target_include_directories(bfs SYSTEM PUBLIC)
target_link_libraries(
    bfs
    gtest gtest_main boost_filesystem boost_system ywen_cpp
)

# Specify the dependencies.
add_dependencies(bfs ywen_cpp)

# ##################################################

# Set the project name.
project(json DESCRIPTION "Verify nlohmann/json behaviors")

# Add the executable.
add_executable(
    json
    "./programs/json.cpp"
)

# Add the include and library directories.
target_include_directories(json SYSTEM PUBLIC)
target_link_libraries(
    json
    gtest gtest_main
)
