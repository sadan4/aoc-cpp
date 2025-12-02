add_executable(day1
  main.cpp
)

target_link_libraries(day1 PRIVATE common fmt::fmt)

set_target_properties(day1 PROPERTIES
  INTERPROCEDURAL_OPTIMIZATION TRUE
)
