//
//  NanaTrip.h
//  nanatrip
//
//  Created by Macbook Air on 7/22/15.
//
//

// Issue:
// didn't detect certain contact like with red or DNA. WHY WON'T YOU DETECT IT!!!!!!!!!! - I think I solved this by get the contact of the static_body instead of the dynamic ones
// sometimes weirdly falls apart, but still connected. All in all, weird - Something to do with drawing method, maybe there's a way to improve it
// situations when nana got stuck in the walls and things - I think there was something in the Box2d header file I can change, a parameter

// TODO:                    more terrain, more obstacle
// * nana size according to screen size
// * highest score
// DNA, which add to HP
// more stuff to InfoLayer
// also the HP system
// * moving red
// increasing difficulty
// bonus item, like pills
// speed limits
// character change
// * terrain change and other
// scoring detail
// music control
// keep screen on


#ifndef nanatrip_NanaTrip_h
#define nanatrip_NanaTrip_h

#include "cocos2d.h"
#include "Box2DHelper.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <SimpleAudioEngine.h>
#include "../C2DXShareSDK/C2DXShareSDK.h"
#include "Randomer.h"

#include "Resources.h"
#include <base/CCUserDefault.h>

#define PTM_RATIO 32

#define DNA_B2RADIUS 36/PTM_RATIO

#define GAME_PLAY 1
#define GAME_PAUSE 2
#define GAME_OVER 3
extern int gameStatus;

// for example, this is a row on the screen [ xxx ]
// when minium colums it would be           [ O O O ]
// maxium colums would be                   [......]
#define MAX_COL 6
#define MIN_COL 3

//  User Data
#define UD_FLOOR 0  // which will actually be NULL
#define UD_NANA 1
#define UD_BADGUY 2
#define UD_DNA 3
#define UD_HAND 4
#define UD_DESTROYED 99

#define IS_DEBUGGING 0

class Entity
{
public:
    Entity(int _t){type = _t;}
    int type;
};

void initStatistic();
void initWinSiz();

b2Vec2 vToB2(cocos2d::Vec2 v);
cocos2d::Vec2 b2ToV(b2Vec2 b);
void logVec2(char* name, cocos2d::Vec2 v);
int randWithBase(int base, int addon);
/**
 * Return false by default.
 * When rand_0_1() < odds, bool switch to true.
 */
bool boolWithOdds(float odds);
void buttonSwitch(cocos2d::ui::Button *button, bool on);
void BackButtonReleased(EventKeyboard::KeyCode code, Event *event);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <platform/android/jni/JniHelper.h>
#include <jni.h>
bool callJavaMethod(char * className, char * method, char * parameter);
#endif

extern std::string deadScreen;
//extern std::string pauseScreen;

extern cocos2d::Size winSiz;
extern float winMidX;
extern float winMidY;
extern cocos2d::Size screenSiz;
extern cocos2d::UserDefault *db;

// TODO: change
extern int score;
extern cocos2d::Vec2 nanap;

extern int pos_score;
extern int eat_score;
extern int dna;

#endif
