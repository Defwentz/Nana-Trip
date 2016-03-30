LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
					\
                   ../../C2DXShareSDK/Android/ShareSDKUtils.cpp \
                   ../../C2DXShareSDK/C2DXShareSDK.cpp \
                   ../../C2DXShareSDK/Android/JSON/CCJSONConverter.cpp \
                   ../../C2DXShareSDK/Android/JSON/cJSON/cJSON.c \
                    \
                   ../../Classes/SplashLayer.cpp \
                   ../../Classes/InterestingLayer.cpp \
                   ../../Classes/JavaOCer.cpp \
                   ../../Classes/SlowerSprite.cpp \
                   ../../Classes/FurSprite.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/AboutLayer.cpp \
                   ../../Classes/Box2DHelper.cpp \
                   ../../Classes/DNASprite.cpp \
                   ../../Classes/GameLayer.cpp \
                   ../../Classes/GLESDebugDraw.cpp \
                   ../../Classes/InfoLayer.cpp \
                   ../../Classes/MoverSprite.cpp \
                   ../../Classes/NanaSprite.cpp \
                   ../../Classes/NanaTrip.cpp \
                   ../../Classes/OverLayer.cpp \
                   ../../Classes/PauseLayer.cpp \
                   ../../Classes/Randomer.cpp \
                   ../../Classes/RedSprite.cpp \
                   ../../Classes/Resources.cpp \
                   ../../Classes/SpriteWithBody.cpp \
                   ../../Classes/StartLayer.cpp \
                   ../../Classes/StayingBlobSprite.cpp \
                   ../../Classes/TerrainSprite.cpp

LOCAL_C_INCLUDES := ../Classes \
					../C2DXShareSDK/Android \
					../C2DXShareSDK/Android/JSON \
					../C2DXShareSDK/Android/JSON/cJSON

LOCAL_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,./prebuilt-mk)
