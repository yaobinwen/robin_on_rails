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
    "./gtest_ext/predicates.cpp"
    "./tmpdir/tmpdir.cpp"
)

# ##################################################

# Set the project name.
project(bfs DESCRIPTION "Verify boost::filesystem behaviors")

# Add the executable.
add_executable(
    bfs
    "./programs/bfs/bfs.cpp"
    "./programs/bfs/create_directory.cpp"
    "./programs/bfs/file_size.cpp"
    "./programs/bfs/path.cpp"
    "./programs/bfs/rename.cpp"
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
project(linux_api DESCRIPTION "Verify Linux APIs behaviors")

# Add the executable.
add_executable(
    linux_api
    "./programs/linux_api/open.cpp"
)

# Add the include and library directories.
target_include_directories(linux_api SYSTEM PUBLIC)
target_link_libraries(
    linux_api
    gtest gtest_main ywen_cpp boost_filesystem boost_system
)

# Specify the dependencies.
add_dependencies(linux_api ywen_cpp)

# ##################################################

# Set the project name.
project(regex DESCRIPTION "Verify C++ regex behaviors")

# Add the executable.
add_executable(
    regex
    "./programs/regex/replace.cpp"
)

# Add the include and library directories.
target_include_directories(regex SYSTEM PUBLIC)
target_link_libraries(
    regex
    gtest gtest_main
)

# ##################################################

# Set the project name.
project(bfs_path_gtest_crash DESCRIPTION "Reproduce bfs::path segmentation fault on gtest")

# Add the executable.
add_executable(
    bfs_path_gtest_crash
    "./programs/bfs_path_gtest_crash.cpp"
)

# Add the include and library directories.
target_include_directories(bfs_path_gtest_crash SYSTEM PUBLIC)
target_link_libraries(
    bfs_path_gtest_crash
    gtest gtest_main boost_filesystem boost_system
)

# ##################################################

# Set the project name.
project(double_delete DESCRIPTION "Verify C++ double delete behaviors")

# Add the executable.
add_executable(
    double_delete
    "./programs/double_delete.cpp"
)

# Add the include and library directories.
target_include_directories(double_delete SYSTEM PUBLIC)

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
