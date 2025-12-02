cmake_minimum_required(VERSION 3.20)
project(advent-of-code VERSION 0.1.0 LANGUAGES CXX)

# Include FetchContent module
include(FetchContent)

# Declare fmtlib dependency
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 10.2.1
)
FetchContent_MakeAvailable(fmt)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Enable Link Time Optimization
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)
if(ipo_supported)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  message(STATUS "IPO/LTO enabled")
else()
  message(WARNING "IPO/LTO not supported: ${ipo_error}")
endif()

# Compiler warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# Add source subdirectory
add_subdirectory(src)
