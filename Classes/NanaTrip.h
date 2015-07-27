//
//  NanaTrip.h
//  nanatrip
//
//  Created by Macbook Air on 7/22/15.
//
//

#ifndef nanatrip_NanaTrip_h
#define nanatrip_NanaTrip_h

#include "cocos2d.h"
#include <Box2D/Box2D.h>
#include "Randomer.h"

#define PTM_RATIO 32

//  User Data
#define UD_FLOOR 0  // which will actually be NULL
#define UD_NANA 1
#define UD_BADGUY 2
#define UD_MED 3

class Entity
{
public:
    Entity(int _t){type = _t;}
    int type;
};

b2Vec2 vToB2(cocos2d::Vec2 v);
cocos2d::Vec2 b2ToV(b2Vec2 b);

extern cocos2d::Size winSiz;
extern float winMidX;
extern float winMidY;

extern int score;

/*cocos2d::Size mwinSize;
cocos2d::Vec2 midWin;

void initWinSize() {
    mwinSize = cocos2d::Director::getInstance()->getWinSize();
    midWin.x = mwinSize.width/2;
    midWin.y = mwinSize.height/2;
}*/
/*
b2Vec2 vToB2(cocos2d::Vec2 v)
{
    return b2Vec2(v.x/PTM_RATIO, v.y/PTM_RATIO);
}

cocos2d::Vec2 b2ToV(b2Vec2 b)
{
    return cocos2d::Vec2(b.x*PTM_RATIO, b.y*PTM_RATIO);
}*/

#endif
