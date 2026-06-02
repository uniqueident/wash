cmake_minimum_required(VERSION 3.28)

project(wash
    VERSION 1.0
    LANGUAGES C
)
set(CMAKE_BUILD_TYPE Debug)
# Set standards
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Add the executable target
file(GLOB_RECURSE SOURCES "Project/src/*.c")
# file(GLOB_RECURSE LIB_SOURCES "Project/lib/*.c")

#library targgets
set(LIBS 
commands_builtin 
stringview
parseargs
vector
)

add_subdirectory(Project/lib)
## Main wash executable
add_executable(wash ${SOURCES})
target_link_libraries(wash ${LIBS})
## Head_nine executable
add_executable(head_nine "proj_head_nine/head_nine.c")
target_link_libraries(head_nine ${LIBS})


## Source - https://stackoverflow.com/a/6595001
## Posted by Adam Bowen, modified by community. See post 'Timeline' for change history
## Retrieved 2026-05-22, License - CC BY-SA 4.0

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
message(STATUS 
"\nRuntime Directory:" ${CMAKE_SOURCE_DIR}/Project/build
"\nLibrary Directory: " ${CMAKE_SOURCE_DIR}/Project/build
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Project/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Project/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
