//
//  NanaTrip.cpp
//  nanatrip
//
//  Created by Macbook Air on 7/27/15.
//
//

#include "NanaTrip.h"

int gameStatus;

cocos2d::Size winSiz;
float winMidX;
float winMidY;

int pos_score;
int eat_score;
int dna;

int getDNA()
{
    return 0;
}

void initStatistic()
{
    pos_score = 0;
    eat_score = 0;
    dna = 0;
}

void initWinSiz()
{
    winSiz = cocos2d::Director::getInstance()->getWinSize();
    winMidX = winSiz.width/2;
    winMidY = winSiz.height/2;
}

b2Vec2 vToB2(cocos2d::Vec2 v)
{
    return b2Vec2(v.x/PTM_RATIO, v.y/PTM_RATIO);
}

cocos2d::Vec2 b2ToV(b2Vec2 b)
{
    return cocos2d::Vec2(b.x*PTM_RATIO, b.y*PTM_RATIO);
}

// resources
std::string res_bg[] = {"bg_1.png"};