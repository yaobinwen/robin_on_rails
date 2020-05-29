cmake_minimum_required(VERSION 3.10)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 17)

# Add the shared library.
add_library(
    semver
    STATIC
    "./SemVer.cpp"
)
add_library(
    demo
    STATIC
    "./Demo.cpp"
)

# Set the project name.
project(deb-packaging-demo DESCRIPTION "Demonstrate Debian packaging process.")

# Add the executable.
add_executable(
    deb-packaging-demo
    "./main.cpp"
)

target_link_libraries(
    deb-packaging-demo
    semver demo
)

# Specify the installation.
install(TARGETS semver DESTINATION lib)
install(TARGETS demo DESTINATION lib)
install(FILES SemVer.hpp DESTINATION include)
install(FILES Demo.hpp DESTINATION include)
install(TARGETS deb-packaging-demo DESTINATION bin)
