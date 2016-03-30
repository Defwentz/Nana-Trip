//
//  PauseLayer.h
//  nanatrip
//
//  Created by Macbook Air on 9/13/15.
//
//

#ifndef __nanatrip__PauseLayer__
#define __nanatrip__PauseLayer__

#include "NanaTrip.h"

class PauseLayer : public cocos2d::Layer
{
    cocos2d::ui::Button* conitnueBtn;
    cocos2d::ui::Button* anotherBtn;
    cocos2d::ui::Button* backBtn;
    
    cocos2d::ui::Button* musicOnBtn;
    cocos2d::ui::Button* musicOffBtn;
    bool isFirst = true;
public:
    //static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(PauseLayer);
    ~PauseLayer();
    void disableAllBtn();

    void continueCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void anotherCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void backCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    EventListenerKeyboard *keyListener;
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    
    void musicOnCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void musicOffCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void switchMusic(bool on);
};

#endif /* defined(__nanatrip__PauseLayer__) */
