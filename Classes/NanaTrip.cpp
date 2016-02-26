//
//  NanaTrip.cpp
//  nanatrip
//
//  Created by Macbook Air on 7/27/15.
//
//

#include "NanaTrip.h"

int gameStatus;
bool vaultDiscovering;

cocos2d::Size winSiz;
float winMidX;
float winMidY;
cocos2d::Size screenSiz;
cocos2d::UserDefault *db;

int score;
cocos2d::Vec2 nanap;
/**
 * Default gravity, b2Vec2(0, -8)
 */
b2Vec2 stdGrav = b2Vec2(0, -9);
//b2Vec2 grav;
int pos_score;
int eat_score;
int dna;

void initStatistic()
{
    score = 0;
    pos_score = 0;
    eat_score = 0;
    dna = 0;
    gameStatus = GAME_PLAY;
    vaultDiscovering = true;
}

void initWinSiz()
{
    auto director = cocos2d::Director::getInstance();
    winSiz = director->getWinSize();
    winMidX = winSiz.width/2;
    winMidY = winSiz.height/2;
    screenSiz = director->getOpenGLView()->getFrameSize();
    db = UserDefault::getInstance();
    //audioEngine->preloadBackgroundMusic("BGMusic01.mp3");
}

b2Vec2 vToB2(cocos2d::Vec2 v)
{
    return b2Vec2(v.x/PTM_RATIO, v.y/PTM_RATIO);
}

cocos2d::Vec2 b2ToV(b2Vec2 b)
{
    return cocos2d::Vec2(b.x*PTM_RATIO, b.y*PTM_RATIO);
}

void logVec2(char* name, cocos2d::Vec2 v) {
    log("%s: (%f, %f)", name, v.x, v.y);
}

int randWithBase(int base, int addon)
{
    return base + rand()%addon;
}

bool boolWithOdds(float odds)
{
    if(rand_0_1() < odds)
        return true;
    else
        return false;
}
void buttonSwitch(cocos2d::ui::Button *button, bool on) {
    if(on) {
        button->setTouchEnabled(true);
        button->setVisible(true);
    } else {
        button->setTouchEnabled(false);
        button->setVisible(false);
    }
}
long getTime() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

std::string deadScreen;// = "dead.png";
//std::string pauseScreen;// = "dead.png";