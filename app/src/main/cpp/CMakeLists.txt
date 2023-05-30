
# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project("studyopengl30")

include_directories(
        glm
        render
        sample
        util
)

file(GLOB src-files
        ${CMAKE_SOURCE_DIR}/*.cpp
        ${CMAKE_SOURCE_DIR}/util/*.cpp
        ${CMAKE_SOURCE_DIR}/sample/*.cpp
        ${CMAKE_SOURCE_DIR}/render/*.cpp

        )


add_library(
        studyopengl30
        SHARED
        ${src-files}
)


find_library(
        log-lib
        log)


target_link_libraries(
        studyopengl30
        EGL
        GLESv3
        ${log-lib})
