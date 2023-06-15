
# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)
#cmake_minimum_required(VERSION 3.4.1)

set(jnilibs "${CMAKE_SOURCE_DIR}/../jniLibs")

# Declares and names the project.

project("studyopengl30")



include_directories(
        glm
        render
        sample
        util
        opencv_3_0_0
        egl
)

file(GLOB src-files
        ${CMAKE_SOURCE_DIR}/*.cpp
        ${CMAKE_SOURCE_DIR}/util/*.cpp
        ${CMAKE_SOURCE_DIR}/sample/*.cpp
        ${CMAKE_SOURCE_DIR}/render/*.cpp
        ${CMAKE_SOURCE_DIR}/egl/*.cpp
        )


add_library(
        studyopengl30
        SHARED
        ${src-files}
)


find_library(
        log-lib
        log)


add_library(opencv_java3 SHARED IMPORTED )
set_target_properties(opencv_java3 PROPERTIES IMPORTED_LOCATION ${jnilibs}/${CMAKE_ANDROID_ARCH_ABI}/libopencv_java3.so)

set(third-party-libs opencv_java3)

target_link_libraries(
        studyopengl30

        ${third-party-libs}
        ${log-lib}
        EGL
        GLESv3
        android
)
