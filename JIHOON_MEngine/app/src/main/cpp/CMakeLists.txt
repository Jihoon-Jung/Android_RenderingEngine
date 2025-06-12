# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("jihoon_mengine")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#
# In this top level CMakeLists.txt, ${CMAKE_PROJECT_NAME} is used to define
# the target library name; in the sub-module's CMakeLists.txt, ${PROJECT_NAME}
# is preferred for the same purpose.
#
# In order to load a library into your app from Java/Kotlin, you must call
# System.loadLibrary() and pass the name of the library defined here;
# for GameActivity/NativeActivity derived applications, the same library name must be
# used in the AndroidManifest.xml file.
add_library(${CMAKE_PROJECT_NAME} SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        native-lib.cpp
        EGLRenderer.cpp
        Shaders.h
        Shaders.cpp
        Interface.h
        Camera.cpp
        Geometry.cpp
        RenderObject.cpp
        Transform.cpp
        Texture.cpp
        Scene.cpp
        Light.cpp
        FirstPersonController.cpp)

# 1. Assimp 소스 디렉토리 지정
set(ASSIMP_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../../../external/assimp)

# 2. Assimp 빌드 디렉토리 지정 (이건 앱 내 CMake가 생성한 빌드 공간에 놓임)
set(ASSIMP_BINARY_DIR ${CMAKE_BINARY_DIR}/assimp-build)

# 3. add_subdirectory는 source와 binary 디렉토리 둘 다 줌
add_subdirectory(${ASSIMP_SOURCE_DIR} ${ASSIMP_BINARY_DIR})

# 4. include 디렉토리 추가
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${ASSIMP_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/../external/eigen
        ${CMAKE_SOURCE_DIR}/../external/stb)

# Specifies libraries CMake should link to your target library. You
# can link libraries from various origins, such as libraries defined in this
# build script, prebuilt third-party libraries, or Android system libraries.
target_link_libraries(${CMAKE_PROJECT_NAME}
        # List libraries link to the target library
        android
        log
        EGL
        GLESv3
        jnigraphics
        assimp)