# libzip-android-ios
build libzip for android/ios  and a sample use c++ to unzip file


1.build android library
(1).download libzip source form https://github.com/nih-at/libzip
unzip flie and use terminal to open file path: /../libzip-1.6.1/android/
open and edit do.sh 

ANDROID_NDK_ROOT=/Users/apple/Library/Android/ndk/android-ndk-r14b #your own ndk path,ndk-18 ndk-14 has been tested

add cmake:-DANDROID_NATIVE_API_LEVEL=android-21 \
          -DANDROID_TOOLCHAIN=gcc #ndk-r18 use clang
 
 run do.sh
 (2)in [install] path will get static library and head file ,contain armeabi-v7a arm64-v8a x86 x86_64 platform ,choose your need
 
 (3)in android project android.mk add libzip library
 
  LIBZIP_PATH :=/../../libzip-1.6.1 #your own path
 
  include $(CLEAR_VARS)
  LOCAL_MODULE := static_libzip
  LOCAL_SRC_FILES := $(LIBZIP_PATH)/lib/libzip.a
  include $(PREBUILT_STATIC_LIBRARY)
  
  LOCAL_C_INCLUDES += $(LIBZIP_PATH)/

  LOCAL_SHARED_LIBRARIES += static_libzip
 
 (4) you can include <zip.h> in your c++ code
 

2.build ios framework
 (1) I direct down others framework form:https://github.com/spemoon/libzip 
    then add into my ios project
   

3.in Unzip_file.cpp,will have c++ code that unzip file to specified location


