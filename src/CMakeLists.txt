# Common utilities library
file(GLOB_RECURSE COMMON_SOURCES CONFIGURE_DEPENDS "common/*.cpp")
add_library(common STATIC ${COMMON_SOURCES})

target_include_directories(common PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(common PUBLIC fmt::fmt)

# Year solutions
add_subdirectory(2025)
