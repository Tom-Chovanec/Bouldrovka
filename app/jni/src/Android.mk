LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL2

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../SDL2/ $(LOCAL_PATH)/../SDL2_image/ $(LOCAL_PATH)/../SDL2_ttf/

# Add your application source files here..:w.
LOCAL_SRC_FILES := main.cpp holds.cpp rendering.cpp ui.cpp utility.cpp files.cpp

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_ttf

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -lOpenSLES -llog -landroid

include $(BUILD_SHARED_LIBRARY)
