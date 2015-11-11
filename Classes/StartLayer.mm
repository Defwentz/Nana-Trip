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

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "ABGameKitHelper.h"
#endif

bool StartLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // game center
    [ABGameKitHelper sharedHelper];
#endif
    
    auto rootNode = CSLoader::createNode("start/start.csb");
    addChild(rootNode);
    
    startBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_start"));
//    storeBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_store"));
    aboutBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_about"));
    //settingBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_setting"));
    musicOnBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_music_on"));
    musicOffBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_music_off"));
    
    //switchMusic(db->getBoolForKey(key_music_status.c_str(), false));
    //CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("BGMusic01.mp3");
    switchMusic(db->getBoolForKey(key_music_status.c_str(), false));
    
    startBtn->addTouchEventListener(CC_CALLBACK_2(StartLayer::startCallback, this));
//    storeBtn->addTouchEventListener(CC_CALLBACK_2(StartLayer::storeCallback, this));
    aboutBtn->addTouchEventListener(CC_CALLBACK_2(StartLayer::aboutCallback, this));
    //settingBtn->addTouchEventListener(CC_CALLBACK_2(StartLayer::settingCallback, this));
    musicOnBtn->addTouchEventListener(CC_CALLBACK_2(StartLayer::musicOnCallback, this));
    musicOffBtn->addTouchEventListener(CC_CALLBACK_2(StartLayer::musicOffCallback, this));
    
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(StartLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
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
        callJavaMethod("org/cocos2dx/mz/AppActivity", "hidebar", "()V");
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
//void StartLayer::settingCallback(Ref* sender, Widget::TouchEventType type)
//{
//    if (type == Widget::TouchEventType::ENDED)
//    {
//        if(settingOn) {
//            musicBtn->setVisible(false);
//        } else {
//            musicBtn->setVisible(true);
//        }
//    }
//}
void StartLayer::switchMusic(bool on) {
    if(on) {
        if(isFirst && !CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("BGMusic01.mp3");
            isFirst = false;
        } else {
            CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
        }
        buttonSwitch(musicOffBtn, false);
        buttonSwitch(musicOnBtn, true);
    } else {
        CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
        buttonSwitch(musicOnBtn, false);
        buttonSwitch(musicOffBtn, true);
    }
    db->setBoolForKey(key_music_status.c_str(), on);
}
void StartLayer::musicOnCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED)
    {
        switchMusic(false);
    }
}
void StartLayer::musicOffCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED)
    {
        switchMusic(true);
    }
}

void StartLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    switch(keyCode)
    {
            //监听返回键
        case EventKeyboard::KeyCode::KEY_ESCAPE:
            Director::getInstance()->end();
            break;
            //监听menu键
        case EventKeyboard::KeyCode::KEY_MENU:
            break;
    }
}