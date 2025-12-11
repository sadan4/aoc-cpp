target_sources(common
    PUBLIC
    filter.hpp
    flatMap.hpp
    fold.hpp
    iota.hpp
    map.hpp
    split.hpp
    sum.hpp
)

target_sources(test
    PUBLIC
    split.test.cpp
)