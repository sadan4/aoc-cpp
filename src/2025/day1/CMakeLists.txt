add_executable(2025_day1
  main.cpp
)

target_link_libraries(2025_day1 PRIVATE common fmt::fmt)

set_target_properties(2025_day1 PROPERTIES
  INTERPROCEDURAL_OPTIMIZATION TRUE
)
