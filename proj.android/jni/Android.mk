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
                   ../../Classes/Layer/SplashLayer.cpp \
                   ../../Classes/Layer/InterestingLayer.cpp \
                   ../../Classes/Layer/AboutLayer.cpp \
                   ../../Classes/Layer/GameLayer.cpp \
                   ../../Classes/Layer/InfoLayer.cpp \
                   ../../Classes/Layer/StartLayer.cpp \
                   ../../Classes/Layer/OverLayer.cpp \
                   ../../Classes/Layer/PauseLayer.cpp \
                   \
                   ../../Classes/Sprite/SlowerSprite.cpp \
                   ../../Classes/Sprite/FurSprite.cpp \
                   ../../Classes/Sprite/DNASprite.cpp \
                   ../../Classes/Sprite/MoverSprite.cpp \
                   ../../Classes/Sprite/NanaSprite.cpp \
                   ../../Classes/Sprite/RedSprite.cpp \
                   ../../Classes/Sprite/SpriteWithBody.cpp \
                   ../../Classes/Sprite/StayingBlobSprite.cpp \
                   ../../Classes/Sprite/TerrainSprite.cpp \
                   \
                   ../../Classes/Util/JavaOCer.cpp \
                   ../../Classes/Util/Box2DHelper.cpp \
                   ../../Classes/Util/GLESDebugDraw.cpp \
                   ../../Classes/Util/Randomer.cpp \
                   ../../Classes/Util/Resources.cpp \
                   \
                   ../../Classes/NanaTrip.cpp \
                   ../../Classes/AppDelegate.cpp

LOCAL_C_INCLUDES := ../Classes \
					../C2DXShareSDK/Android \
					../C2DXShareSDK/Android/JSON \
					../C2DXShareSDK/Android/JSON/cJSON

LOCAL_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,./prebuilt-mk)
