# Common utilities library
add_library(common STATIC)
target_sources(
    common
    PUBLIC
    Base.cpp
    Base.hpp
)
target_sources(test
  PUBLIC
  sanity.test.cpp
)
add_subdirectory(util)
add_subdirectory(deps)

target_include_directories(common PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(common PUBLIC fmt::fmt)

set_target_properties(common PROPERTIES
  INTERPROCEDURAL_OPTIMIZATION TRUE
)