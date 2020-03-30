cmake_minimum_required(VERSION 3.17)
project(StreamRetro
        VERSION 0.1.0
        DESCRIPTION "libretro frontend in C++ for streaming purposes."
        LANGUAGES CXX)

# set compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# disallow compiler specific extensions
set(CMAKE_CXX_EXTENSIONS OFF)

# add streamRetro target as executable and tell them where source files are
set(INCLUDE_LIST source/main.cpp source/LibRetroGameWrapper.cpp source/Core.cpp)

add_executable(streamRetro ${INCLUDE_LIST})
target_include_directories(streamRetro PUBLIC source)
target_link_libraries(streamRetro PRIVATE dl GL GLEW glfw asound)
set_target_properties(streamRetro
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
