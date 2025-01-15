LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

# Add your application source files here...
LOCAL_SRC_FILES := \
	app.cpp icon.cpp main.cpp renderer.cpp resource_manager.cpp scene.cpp object.cpp image.cpp simple_button.cpp text.cpp colors.cpp boulder_selection.cpp hold.cpp

SDL_PATH := ../SDL  # SDL

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include  # SDL

LOCAL_SHARED_LIBRARIES := SDL3 SDL3_image SDL3_ttf

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -lOpenSLES -llog -landroid  # SDL

include $(BUILD_SHARED_LIBRARY)
