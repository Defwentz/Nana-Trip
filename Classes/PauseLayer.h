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
//    cocos2d::ui::Button* backBtn;
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(PauseLayer);
    void disableAllBtn();

    void continueCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void anotherCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
//    void backCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
};

#endif /* defined(__nanatrip__PauseLayer__) */
