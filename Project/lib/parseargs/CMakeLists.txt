add_library(parseargs STATIC
    parseargs.c
    parseargs.h
)
target_include_directories(parseargs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(parseargs PUBLIC stringview vector)