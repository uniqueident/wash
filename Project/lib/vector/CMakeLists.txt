add_library(vector STATIC
    vector.c
    vector.h
)
target_include_directories(vector PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})