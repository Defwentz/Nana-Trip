//
//  PauseLayer.cpp
//  nanatrip
//
//  Created by Macbook Air on 9/13/15.
//
//

#include "PauseLayer.h"
#include "GameLayer.h"
#include "StartLayer.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <platform/android/jni/JniHelper.h>
#include <jni.h>
#endif

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace cocos2d::ui;

Scene* PauseLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = PauseLayer::create();
    scene->addChild(layer);
    return scene;
}

bool PauseLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("another_pause/NewPauseLayer.csb");
    addChild(rootNode);
    
    Text* scoreTxt = dynamic_cast<Text*>(rootNode->getChildByName("Text_score"));
    scoreTxt->setString(StringUtils::format("%d", score));
    
    conitnueBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_continue"));
    anotherBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_new"));
    //backBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_back"));
    conitnueBtn->addTouchEventListener(CC_CALLBACK_2(PauseLayer::continueCallback, this));
    anotherBtn->addTouchEventListener(CC_CALLBACK_2(PauseLayer::anotherCallback, this));
    //backBtn->addTouchEventListener(CC_CALLBACK_2(PauseLayer::backCallback, this));
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    // Andorid
    log("android platform!");
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t, "xx/jnitst/MyJavaClass", "dosth", "()V")) {
        log("call function succeed!!!!!!!!!!");
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        //t.env->DeleteLocalRef(t.classID);
    }
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    // iOS
    
#endif
    return true;
}


void PauseLayer::continueCallback(Ref *sender, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED)
    {
        Director::getInstance()->popScene();
        NotificationCenter::getInstance()->postNotification("defaultCallback");
    }
}
void PauseLayer::anotherCallback(Ref *sender, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED)
    {
        // I don't know what to do. Do I reset it? Do simply don't care about it?
        // The answer is, I simply don't care.
        // Director::getInstance()->popScene();
        Director::getInstance()->replaceScene(GameLayer::createScene());
    }
}
//void PauseLayer::backCallback(Ref* sender, Widget::TouchEventType type)
//{
//    if (type == Widget::TouchEventType::ENDED)
//    {
//        //Director::getInstance()->popScene();
//        Director::getInstance()->replaceScene(StartLayer::createScene());
//    }
//}