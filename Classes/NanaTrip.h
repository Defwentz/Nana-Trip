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
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#define PTM_RATIO 32

#define DNA_B2RADIUS 36/PTM_RATIO

#define GAME_PLAY 1
#define GAME_PAUSE 2
#define GAME_OVER 3

extern int gameStatus;

//  User Data
#define UD_FLOOR 0  // which will actually be NULL
#define UD_NANA 1
#define UD_BADGUY 2
#define UD_DNA 3
#define UD_DESTROYED 99

// for example, this is a row on the screen [ xxx ]
// when minium colums it would be     [ O O O ]
// maxium colums would be            [.......]
#define MAX_COL 6
#define MIN_COL 3

class Entity
{
public:
    Entity(int _t){type = _t;}
    int type;
};

int getDNA();
void initStatistic();
void initWinSiz();

b2Vec2 vToB2(cocos2d::Vec2 v);
cocos2d::Vec2 b2ToV(b2Vec2 b);

extern cocos2d::Size winSiz;
extern float winMidX;
extern float winMidY;

// TODO: change
extern int pos_score;
extern int eat_score;
extern int dna;

#endif
