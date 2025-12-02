add_executable(2024_day1
  main.cpp
)

target_link_libraries(2024_day1 PRIVATE common fmt::fmt)

set_target_properties(2024_day1 PROPERTIES
  INTERPROCEDURAL_OPTIMIZATION TRUE
)
