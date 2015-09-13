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
    pauseBtn->loadTextures("button_pause_small.png", "button_pause_small.png");
    pauseBtn->cocos2d::Node::setPosition(winSiz.width - 50, winSiz.height - 50);
    pauseBtn->setTouchEnabled(true);
    pauseBtn->addTouchEventListener(CC_CALLBACK_2(InfoLayer::pauseCallback, this));
    this->addChild(pauseBtn);
    
    NotificationCenter::getInstance()->
    addObserver(this, callfuncO_selector(InfoLayer::defaultCallBack), "defaultCallback", NULL);
    
    return true;
}
InfoLayer::~InfoLayer() {
    NotificationCenter::getInstance()->removeObserver(this, "defaultCallback");
}
void InfoLayer::reset() {
    old_pos_score = 0;
    scoreLabel->setString(StringUtils::format("0"));
}

void InfoLayer::update()
{
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
    gameStatus = GAME_PAUSE;
    Device::setAccelerometerEnabled(false);
    pauseBtn->setTouchEnabled(false);
    Director::getInstance()->pushScene(PauseLayer::createScene());
}
void InfoLayer::defaultCallBack(cocos2d::Ref *pSender) {
    gameStatus = GAME_PLAY;
    Device::setAccelerometerEnabled(true);
    pauseBtn->setTouchEnabled(true);
}