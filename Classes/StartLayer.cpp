//
//  StartLayer.cpp
//  nanatrip
//
//  Created by Macbook Air on 7/30/15.
//
//

#include "StartLayer.h"
#include "GameLayer.h"
#include "AboutLayer.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <platform/android/jni/JniHelper.h>
#include <jni.h>
#endif


USING_NS_CC;

using namespace cocostudio::timeline;
using namespace cocos2d::ui;

Scene* StartLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = StartLayer::create();
    scene->addChild(layer);
    return scene;
}

bool StartLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("start/start.csb");
    addChild(rootNode);
    
    startBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_start"));
//    storeBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_store"));
    aboutBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_about"));
    startBtn->addTouchEventListener(CC_CALLBACK_2(StartLayer::startCallback, this));
//    storeBtn->addTouchEventListener(CC_CALLBACK_2(StartLayer::storeCallback, this));
    aboutBtn->addTouchEventListener(CC_CALLBACK_2(StartLayer::aboutCallback, this));
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t, "org/cocos2dx/mz/AppActivity", "showbar", "()V")) {
        log("call do nothing succeed!!!!!!!!!!");
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        //t.env->DeleteLocalRef(t.classID);
    }
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
#endif
    
    return true;
}

void StartLayer::startCallback(Ref* sender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
        // Andorid
        log("android platform!");
        JniMethodInfo t;
        if(JniHelper::getStaticMethodInfo(t, "org/cocos2dx/mz/AppActivity", "hidebar", "()V")) {
            log("call do nothing succeed!!!!!!!!!!");
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            //t.env->DeleteLocalRef(t.classID);
        }
        
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        
        // iOS
        
#endif
        Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
        Director::getInstance()->replaceScene(GameLayer::createScene());
    }
}
//void StartLayer::storeCallback(Ref* sender, Widget::TouchEventType type)
//{
//    if (type == Widget::TouchEventType::ENDED)
//    {
//        //Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
//        //CCDirector::getInstance()->replaceScene(GameLayer::createScene());
//    }
//}
void StartLayer::aboutCallback(Ref* sender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
        Director::getInstance()->replaceScene(AboutLayer::createScene());
    }
}