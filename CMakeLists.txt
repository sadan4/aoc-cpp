message(STATUS "Using CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
message(STATUS "Using CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "Using CMAKE_CXX_COMPILER_AR: ${CMAKE_CXX_COMPILER_AR}")
message(STATUS "Using CMAKE_CXX_COMPILER_RANLIB: ${CMAKE_CXX_COMPILER_RANLIB}")
cmake_minimum_required(VERSION 3.20)
project(advent-of-code VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(IS_CLANG ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
set(IS_GCC ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")

# Include FetchContent module
include(FetchContent)
include(CheckCXXCompilerFlag)

# # Declare fmtlib dependency
# FetchContent_Declare(
#         fmt
#         GIT_REPOSITORY https://github.com/fmtlib/fmt.git
#         GIT_TAG 10.2.1
# )
# FetchContent_MakeAvailable(fmt)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

configure_file("${Catch2_SOURCE_DIR}/extras/gdbinit" "gdbinit" COPYONLY)
configure_file("${Catch2_SOURCE_DIR}/extras/lldbinit" "lldbinit" COPYONLY)

add_executable(test)
target_link_libraries(test PRIVATE Catch2::Catch2WithMain common)

check_cxx_compiler_flag(-fvar-tracking HAS_VAR_TRACKING)
if (HAS_VAR_TRACKING) 
    target_compile_options(test PRIVATE -fvar-tracking)
endif()

if (NOT IS_CLANG)
    target_compile_options(test PRIVATE -fno-eliminate-unused-debug-symbols)
endif()

check_cxx_compiler_flag(-fno-merge-debug-strings HAS_NO_MERGE_DEBUG_STRINGS)
if (HAS_NO_MERGE_DEBUG_STRINGS)
    target_compile_options(test PRIVATE -fno-merge-debug-strings)
endif()

if (WIN32)
    target_compile_options(test PRIVATE -g)
else()
    check_cxx_compiler_flag(-ggdb3 HAS_G_GDB_3)
    if (HAS_G_GDB_3)
        target_compile_options(test PRIVATE -ggdb3)
    else()
        message(WARNING "Compiler does not support -ggdb3, trying lower debug levels")
        check_cxx_compiler_flag(-ggdb HAS_G_GDB)
        check_cxx_compiler_flag(-g3 HAS_G_3)
        check_cxx_compiler_flag(-g HAS_G)
        if (HAS_G_GDB)
            target_compile_options(test PRIVATE -ggdb)
        elseif(HAS_G_3)
            target_compile_options(test PRIVATE -g3)
        elseif(HAS_G)
            target_compile_options(test PRIVATE -g)
        else()
            message(WARNING "Compiler does not support any debug flags, debug info may be limited")
        endif()
    endif()
endif()

target_compile_options(test PRIVATE -O0)



# Set build type to Release if not specified
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

# Enable Link Time Optimization
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)
if (ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    message(STATUS "IPO/LTO enabled")
else ()
    message(WARNING "IPO/LTO not supported: ${ipo_error}")
endif ()

# Compiler warnings
add_compile_options(-Wall -Wextra -Wpedantic)
add_compile_options(-ggdb3)

# Add source subdirectory
add_subdirectory(src)

# uncomment to profile
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og")
# add_compile_options(-fno-omit-frame-pointer -mno-omit-leaf-frame-pointer)

