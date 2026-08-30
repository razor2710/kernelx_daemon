LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := CAESAR.sh
LOCAL_CPPFLAGS := -w -std=c++17
LOCAL_CFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR

LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/Android_draw
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/Android_touch
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/Android_vulkan
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/ImGui
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/native_surface
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/Tools

LOCAL_SRC_FILES := \
    src/main.cpp \
    src/Android_draw/draw.cpp \
    src/Android_touch/touch.cpp \
    src/Android_vulkan/vulkan_wrapper.cpp \
    src/Android_vulkan/VulkanGraphics.cpp \
    src/ImGui/GUI_Custom.cpp \
    src/ImGui/imgui.cpp \
    src/ImGui/imgui_draw.cpp \
    src/ImGui/imgui_impl_android.cpp \
    src/ImGui/imgui_impl_vulkan.cpp \
    src/ImGui/imgui_tables.cpp \
    src/ImGui/imgui_widgets.cpp \
    src/ImGui/stb_image.cpp \
    src/res/cJSON.c

LOCAL_LDFLAGS := -llog -landroid

include $(BUILD_EXECUTABLE)
