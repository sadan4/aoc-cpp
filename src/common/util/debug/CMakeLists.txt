target_sources(common
    PUBLIC
    ColoredString.cpp
    ColoredString.hpp
    TerminalCode.hpp
)

target_sources(test
    PRIVATE
    ColoredString.test.cpp
)