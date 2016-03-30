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

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace cocos2d::ui;

//Scene* PauseLayer::createScene()
//{
//    auto scene = Scene::create();
//    auto layer = PauseLayer::create();
//    scene->addChild(layer);
//    return scene;
//}

bool PauseLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("another_pause/NewPauseLayer.csb");
    addChild(rootNode);
    
//    Sprite* bgSprite = dynamic_cast<Sprite*>(rootNode->getChildByName("bg"));
//    Texture2D* texture = Director::getInstance()->getTextureCache()->addImage(pauseScreen);
//    bgSprite->setTexture(texture);
    
    Text* scoreTxt = dynamic_cast<Text*>(rootNode->getChildByName("Text_score"));
    scoreTxt->setString(StringUtils::format("%d", score));
    
    conitnueBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_continue"));
    anotherBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_new"));
    backBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_back"));
    conitnueBtn->setTouchEnabled(true);
    anotherBtn->setTouchEnabled(true);
    backBtn->setTouchEnabled(true);
    conitnueBtn->addTouchEventListener(CC_CALLBACK_2(PauseLayer::continueCallback, this));
    anotherBtn->addTouchEventListener(CC_CALLBACK_2(PauseLayer::anotherCallback, this));
    backBtn->addTouchEventListener(CC_CALLBACK_2(PauseLayer::backCallback, this));
    
    musicOnBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_music_on"));
    musicOffBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_music_off"));
    switchMusic(db->getBoolForKey(key_music_status.c_str()));
    musicOnBtn->addTouchEventListener(CC_CALLBACK_2(PauseLayer::musicOnCallback, this));
    musicOffBtn->addTouchEventListener(CC_CALLBACK_2(PauseLayer::musicOffCallback, this));
    
    keyListener = EventListenerKeyboard::create();
    keyListener->onKeyReleased = CC_CALLBACK_2(PauseLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

    return true;
}

PauseLayer::~PauseLayer() {
    disableAllBtn();
}

void PauseLayer::disableAllBtn() {
    conitnueBtn->setTouchEnabled(false);
    anotherBtn->setTouchEnabled(false);
    backBtn->setTouchEnabled(false);
}

void PauseLayer::continueCallback(Ref *sender, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::BEGAN)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("button_sound.mp3");
    }
    if (type == Widget::TouchEventType::ENDED)
    {
        disableAllBtn();
        _eventDispatcher->removeEventListener(keyListener);
        this->removeFromParent();
        //Director::getInstance()->popScene();
        NotificationCenter::getInstance()->postNotification("defaultCallback");
    }
}
void PauseLayer::anotherCallback(Ref *sender, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::BEGAN)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("button_sound.mp3");
    }
    if (type == Widget::TouchEventType::ENDED)
    {
        // I don't know what to do. Do I reset it? Do simply don't care about it?
        // The answer is, I simply don't care.
        // Director::getInstance()->popScene();
        Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
        Director::getInstance()->replaceScene(GameLayer::createScene());
    }
}
void PauseLayer::backCallback(Ref* sender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::BEGAN)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("button_sound.mp3");
    }
    if (type == Widget::TouchEventType::ENDED)
    {
        //Director::getInstance()->popScene();
        _eventDispatcher->removeEventListener(keyListener);
        Director::getInstance()->replaceScene(StartLayer::createScene());
    }
}
void PauseLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    switch(keyCode)
    {
            //监听返回键
        case EventKeyboard::KeyCode::KEY_ESCAPE:
            continueCallback(NULL, Widget::TouchEventType::ENDED);
            break;
            //监听menu键
        case EventKeyboard::KeyCode::KEY_MENU:
            break;
            
    }
}

void PauseLayer::switchMusic(bool on) {
    if(on) {
        if(isFirst && !CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) {
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("menu_music.m4a", true);
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
void PauseLayer::musicOnCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::BEGAN)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("button_sound.mp3");
    }
    if (type == Widget::TouchEventType::ENDED)
    {
        switchMusic(false);
    }
}
void PauseLayer::musicOffCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::BEGAN)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("button_sound.mp3");
    }
    if (type == Widget::TouchEventType::ENDED)
    {
        switchMusic(true);
    }
}