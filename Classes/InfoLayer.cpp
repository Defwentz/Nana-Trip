//
//  InfoLayer.cpp
//  nanatrip
//
//  Created by Macbook Air on 7/27/15.
//
//

#include "InfoLayer.h"

USING_NS_CC;

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
    
    old_score = score;
    
    scheduleUpdate();
    return true;
}

void InfoLayer::update(float dt)
{
    if(score > old_score) {
        scoreLabel->setString(StringUtils::format("%d", score));
        old_score = score;
    }
    
}