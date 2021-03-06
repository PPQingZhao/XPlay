# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# configure import libs
set(distribution_DIR ${CMAKE_SOURCE_DIR}/../distribution)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

file(GLOB SOURCES
    "src/main/cpp/*.cpp"
    "src/main/cpp/observer/*.cpp"
    "src/main/cpp/thread/*.cpp"
    "src/main/cpp/decode/*.cpp"
    "src/main/cpp/data/*.cpp"
    "src/main/cpp/demux/*.cpp"
    "src/main/cpp/log/*.cpp"
    "src/main/cpp/videoviewer/*.cpp"
    "src/main/cpp/audio/*.cpp"
    "src/main/cpp/player/*.cpp"
    "src/main/cpp/builder/*.cpp"
    "src/main/cpp/proxy/*.cpp"
    )

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             ${SOURCES} )

#设置ffmpeg头文件路径
#方式一:
#include_directories(${distribution_DIR}/include)
#方式二:
target_include_directories(native-lib PRIVATE ${distribution_DIR}/include)

#创建一个.so库avcodec，导入方式
add_library(avcodec SHARED IMPORTED)
add_library(avfilter SHARED IMPORTED)
add_library(avformat SHARED IMPORTED)
add_library(avutil SHARED IMPORTED)
add_library(swresample SHARED IMPORTED)
add_library(swscale SHARED IMPORTED)
#设置库avcodec所需的源文件
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${distribution_DIR}/libs/${ANDROID_ABI}/libavcodec.so)
set_target_properties(avfilter PROPERTIES IMPORTED_LOCATION ${distribution_DIR}/libs/${ANDROID_ABI}/libavfilter.so)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION ${distribution_DIR}/libs/${ANDROID_ABI}/libavformat.so)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION ${distribution_DIR}/libs/${ANDROID_ABI}/libavutil.so)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION ${distribution_DIR}/libs/${ANDROID_ABI}/libswresample.so)
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION ${distribution_DIR}/libs/${ANDROID_ABI}/libswscale.so)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib
              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib
                       # Links the target library to the library
                       # included in the NDK.
                       avcodec avfilter avformat avutil swresample swscale
                       android
                       OpenSLES GLESv2 EGL
                       ${log-lib})

#将.so文件打包到apk中
add_custom_command(TARGET native-lib PRE_BUILD
                   COMMAND "${CMAKE_COMMAND}" -E
                   copy
                   "${distribution_DIR}/libs/${ANDROID_ABI}/libavcodec.so"
                   "${distribution_DIR}/libs/${ANDROID_ABI}/libavfilter.so"
                   "${distribution_DIR}/libs/${ANDROID_ABI}/libavformat.so"
                   "${distribution_DIR}/libs/${ANDROID_ABI}/libavutil.so"
                   "${distribution_DIR}/libs/${ANDROID_ABI}/libswresample.so"
                   "${distribution_DIR}/libs/${ANDROID_ABI}/libswscale.so"
                   "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
