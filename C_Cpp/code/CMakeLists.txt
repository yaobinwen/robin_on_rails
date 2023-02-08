cmake_minimum_required(VERSION 3.10)

# Specify the C standard
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 11)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 17)

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
    "./programs/bfs/absolute.cpp"
    "./programs/bfs/bfs.cpp"
    "./programs/bfs/copy_directory.cpp"
    "./programs/bfs/create_directory.cpp"
    "./programs/bfs/directory_iterator.cpp"
    "./programs/bfs/file_size.cpp"
    "./programs/bfs/is_type.cpp"
    "./programs/bfs/path.cpp"
    "./programs/bfs/relative.cpp"
    "./programs/bfs/rename.cpp"
)

# Add the include and library directories.
target_include_directories(bfs SYSTEM PUBLIC)
target_link_libraries(
    bfs
    gtest gtest_main boost_filesystem boost_system pthread ywen_cpp
)

# Specify the dependencies.
add_dependencies(bfs ywen_cpp)

# ##################################################

# Set the project name.
# We treat the thread demo code separately because thread code takes a bit time
# to finish running. It becomes annoying when what I really care about is non-
# thread demo code but have to wait for the thread code.
project(
  cpp_demo_thread
  DESCRIPTION "Demonstrate C++ language and standard library features (thread specifically)")

# Add the executable.
add_executable(
    cpp_demo_thread
    "./programs/cpp_demo/thread.cpp"
)

# Add the include and library directories.
target_include_directories(cpp_demo_thread SYSTEM PUBLIC)
target_link_libraries(
  cpp_demo_thread
    gtest gtest_main boost_filesystem boost_system pthread
)

# ##################################################

# Set the project name.
project(cpp_demo DESCRIPTION "Demonstrate C++ language and standard library features")

# Add the executable.
add_executable(
    cpp_demo
    "./programs/cpp_demo/array.cpp"
    "./programs/cpp_demo/chrono.cpp"
    "./programs/cpp_demo/exceptions.cpp"
    "./programs/cpp_demo/iterator.cpp"
    "./programs/cpp_demo/override.cpp"
    "./programs/cpp_demo/shared_ptr.cpp"
    "./programs/cpp_demo/string_view.cpp"
    "./programs/cpp_demo/string.cpp"
    "./programs/cpp_demo/type_traits.cpp"
    "./programs/cpp_demo/variadic.cpp"
    "./programs/cpp_demo/vector.cpp"
)

# Add the include and library directories.
target_include_directories(cpp_demo SYSTEM PUBLIC)
target_link_libraries(
    cpp_demo
    gtest gtest_main boost_filesystem boost_system pthread
)

# ##################################################

# Set the project name.
project(linux_api DESCRIPTION "Verify Linux APIs behaviors")

# Add the executable.
add_executable(
    linux_api
    "./programs/linux_api/open.cpp"
    "./programs/linux_api/rename.cpp"
)

# Add the include and library directories.
target_include_directories(linux_api SYSTEM PUBLIC)
target_link_libraries(
    linux_api
    gtest gtest_main ywen_cpp boost_filesystem boost_system pthread
)

# Specify the dependencies.
add_dependencies(linux_api ywen_cpp)

# ##################################################

# Set the project name.
project(regex DESCRIPTION "Verify C++ regex behaviors")

# Add the executable.
add_executable(
    regex
    "./programs/regex/parse.cpp"
    "./programs/regex/replace.cpp"
)

# Add the include and library directories.
target_include_directories(regex SYSTEM PUBLIC)
target_link_libraries(
    regex
    gtest gtest_main pthread
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
    gtest gtest_main boost_filesystem boost_system pthread
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
project(file_demo DESCRIPTION "Demonstrate how to manipulate a file")

# Add the executable.
add_executable(
    file_demo
    "./programs/file_demo/file_exists.cpp"
    "./programs/file_demo/flagbits.cpp"
    "./programs/file_demo/read.cpp"
)

# Add the include and library directories.
target_include_directories(file_demo SYSTEM PUBLIC)
target_link_libraries(
    file_demo
    gtest gtest_main boost_filesystem boost_system pthread ywen_cpp
)

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
    gtest gtest_main pthread
)

# ##################################################

# Set the project name.
project(boost_log_demo DESCRIPTION "Demonstrate how to use Boost Log")

# Make sure the required Boost packages can be found.
find_package(Boost COMPONENTS REQUIRED
  log
  log_setup
  system
  thread
)

add_definitions(-DBOOST_LOG_DYN_LINK)

# Add the executable.
add_executable(
    boost_log_demo
    "./programs/boost_log.cpp"
)

# Add the include and library directories.
target_include_directories(boost_log_demo SYSTEM PUBLIC)
target_link_libraries(
    boost_log_demo
    boost_log
    boost_system
    boost_thread
    gtest gtest_main pthread
)

# ##################################################

# Set the project name.
project(boost_process_demo DESCRIPTION "Demonstrate how to use Boost Process")

# Make sure Boost can be found.
find_package(Boost
    REQUIRED
    COMPONENTS filesystem system
)

# Add the executable.
add_executable(
    boost_process_demo
    "./programs/boost_process.cpp"
)

# Add the include and library directories.
target_include_directories(boost_process_demo SYSTEM PUBLIC)
target_link_libraries(
    boost_process_demo
    ${Boost_LIBRARIES}
    gtest gtest_main pthread
)

# ##################################################

# Set the project name.
project(fp_compare_demo DESCRIPTION "Demonstrate how to compare floating-point numbers")

# Add the executable.
add_executable(
    fp_compare_demo
    "./programs/fp_compare.cpp"
)

# Add the include and library directories.
target_include_directories(fp_compare_demo SYSTEM PUBLIC)
target_link_libraries(
    fp_compare_demo
    gtest gtest_main pthread
)
