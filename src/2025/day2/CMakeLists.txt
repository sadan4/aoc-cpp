add_executable(day2
  main.cpp
)

target_link_libraries(day2 PRIVATE common fmt::fmt)

set_target_properties(day2 PROPERTIES
  INTERPROCEDURAL_OPTIMIZATION TRUE
)
