ifeq ($(TARGET_USE_LIBHOOK),true)
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := hook.c
LOCAL_MODULE := libhook
LOCAL_SHARED_LIBRARIES := liblog libc libdl
LOCAL_MODULE_TAGS := optional
LOCAL_VENDOR_MODULE := true
include $(BUILD_SHARED_LIBRARY)
endif # TARGET_USE_LIBHOOK
