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
    
    score = old_pos_score = 0;
    
    scheduleUpdate();
    return true;
}

void InfoLayer::update(float dt)
{
    if(gameStatus == GAME_OVER) {
        old_pos_score = 0;
        score = 0;
        return;
    }
    if(pos_score > old_pos_score) {
        score = pos_score + eat_score;
        scoreLabel->setString(StringUtils::format("%d", score));
        old_pos_score = pos_score;
    }
    
}