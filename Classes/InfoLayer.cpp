//
//  InfoLayer.cpp
//  nanatrip
//
//  Created by Macbook Air on 7/27/15.
//
//

#include "InfoLayer.h"

USING_NS_CC;

using namespace cocos2d::ui;

bool InfoLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    scoreLabel = Label::create();
    scoreLabel->setPosition(winMidX, winSiz.height - 100);
    scoreLabel->setColor(Color3B::WHITE);
    scoreLabel->setScale(10);
    this->addChild(scoreLabel);
    
    old_pos_score = 0;
    scoreLabel->setString(StringUtils::format("0"));
    
    pauseBtn = Button::create();
    pauseBtn->setTouchEnabled(true);
    pauseBtn->loadTextures("button_pause_small.png", "button_pause_small.png");
    pauseBtn->cocos2d::Node::setPosition(winSiz.width - 50, winSiz.height - 50);
    pauseBtn->addTouchEventListener(this, toucheventselector(InfoLayer::pauseBtnTouched));
    this->addChild(pauseBtn);
    
    scheduleUpdate();
    return true;
}

void InfoLayer::update(float dt)
{
    if(pos_score > old_pos_score) {
        scoreLabel->setString(StringUtils::format("%d", pos_score + eat_score));
        old_pos_score = pos_score;
    }
    
}

void InfoLayer::reset() {
    old_pos_score = 0;
    scoreLabel->setString(StringUtils::format("0"));
}

void InfoLayer::pauseBtnTouched(Object *pSender, cocos2d::ui::TouchEventType type)
{
    Button* butten = (Button*)pSender;
    unsigned int tag = butten->getTag();
    switch (tag) {
        case TOUCH_EVENT_BEGAN:
            gameStatus = GAME_PAUSE;
            break;
        default:
            break;
    }
    
}