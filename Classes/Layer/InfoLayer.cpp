//
//  InfoLayer.cpp
//  nanatrip
//
//  Created by Macbook Air on 7/27/15.
//
//

#include "InfoLayer.h"
#include "PauseLayer.h"

USING_NS_CC;

using namespace cocos2d::ui;

bool InfoLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    scoreLabel = Label::createWithTTF(StringUtils::format("0"), "fonts/Marker Felt.ttf", 60);
    scoreLabel->setPosition(winMidX, winSiz.height - 100);
    scoreLabel->setColor(Color3B::WHITE);
    this->addChild(scoreLabel);
    old_pos_score = 0;
    
    pauseBtn = Button::create();
    pauseBtn->loadTextures("button_pause.png", "button_pause.png");
    //pauseBtn->cocos2d::Node::setScale(1.5);
    pauseBtn->cocos2d::Node::setPosition(winSiz.width - 100, winSiz.height - 100);
    pauseBtn->setTouchEnabled(true);
    pauseBtn->addTouchEventListener(CC_CALLBACK_2(InfoLayer::pauseCallback, this));
    this->addChild(pauseBtn);
    
//    soundBtn = Button::create();
//    soundBtn->loadTextures("homepage/button_sound_off.png", "homepage/button_sound_off.png");
//    pauseBtn->cocos2d::Node::setPosition(winSiz.width - 50, winSiz.height - 50);
//    pauseBtn->setTouchEnabled(true);
//    pauseBtn->addTouchEventListener(CC_CALLBACK_2(InfoLayer::pauseCallback, this));
//    this->addChild(pauseBtn);
    
    NotificationCenter::getInstance()->
    addObserver(this, callfuncO_selector(InfoLayer::defaultCallback), "defaultCallback", NULL);
    NotificationCenter::getInstance()->
    addObserver(this, callfuncO_selector(InfoLayer::anotherPauseCallback), "another_pause", NULL);
    
    return true;
}
InfoLayer::~InfoLayer() {
    NotificationCenter::getInstance()->removeObserver(this, "defaultCallback");
    NotificationCenter::getInstance()->removeObserver(this, "another_pause");
}
void InfoLayer::reset() {
    old_pos_score = 0;
    scoreLabel->setString(StringUtils::format("0"));
}

void InfoLayer::update()
{
    if(gameStatus == GAME_PAUSE)return;
    if(pos_score > old_pos_score) {
        old_pos_score = pos_score;
    }
    else {
        pos_score = old_pos_score;
    }
    score = pos_score + eat_score;
    scoreLabel->setString(StringUtils::format("%d", score));
}

void InfoLayer::pauseCallback(Ref *sender, Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::BEGAN)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("button_sound.mp3");
    }
    if (type == Widget::TouchEventType::ENDED) {
        CocosDenshion::SimpleAudioEngine::getInstance()->pauseAllEffects();
        NotificationCenter::getInstance()->postNotification("pause_sign");
        gameStatus = GAME_PAUSE;
        Device::setAccelerometerEnabled(false);
        buttonSwitch(pauseBtn, false);
        scoreLabel->setVisible(false);
        this->getParent()->addChild(PauseLayer::create(), ZORDER_PAUSELAYER);
    }
}
void InfoLayer::soundCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    
}
void InfoLayer::defaultCallback(cocos2d::Ref *pSender) {
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
    gameStatus = GAME_PLAY;
    Device::setAccelerometerEnabled(true);
    buttonSwitch(pauseBtn, true);
    scoreLabel->setVisible(true);
}

void InfoLayer::anotherPauseCallback(cocos2d::Ref *pSender) {
    pauseCallback(NULL, cocos2d::ui::Widget::TouchEventType::ENDED);
}