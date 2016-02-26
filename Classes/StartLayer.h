//
//  StartLayer.h
//  nanatrip
//
//  Created by Macbook Air on 7/30/15.
//
//

#ifndef __nanatrip__StartLayer__
#define __nanatrip__StartLayer__

#include "NanaTrip.h"

class StartLayer : public cocos2d::Layer
{
    cocos2d::ui::Button* startBtn;
//    cocos2d::ui::Button* storeBtn;
    cocos2d::ui::Button* aboutBtn;
    //cocos2d::ui::Button* settingBtn;
    //bool settingOn;
    bool isFirst = true;
    cocos2d::ui::Button* musicOnBtn;
    cocos2d::ui::Button* musicOffBtn;
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(StartLayer);
    
    void startCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
//    void storeCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void aboutCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    //void settingCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void musicOnCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void musicOffCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    void _exit();
    long _exitTime = 0;
#endif
    
    void switchMusic(bool on);
};

#endif /* defined(__nanatrip__StartLayer__) */
