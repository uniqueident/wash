add_library(commands_builtin STATIC
    builtin.c
    builtin.h
)
target_include_directories(commands_builtin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(commands_builtin PUBLIC vector)