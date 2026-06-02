add_library(stringview STATIC
    stringview.c
    stringview.h
)
target_include_directories(stringview PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})