add_executable(day3
  main.cpp
)

target_link_libraries(day3 PRIVATE common)

set_target_properties(day3 PROPERTIES
  INTERPROCEDURAL_OPTIMIZATION TRUE
)
