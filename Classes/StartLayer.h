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
//    cocos2d::ui::Button* aboutBtn;
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(StartLayer);
    
    void startCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
//    void storeCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
//    void aboutCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
};

#endif /* defined(__nanatrip__StartLayer__) */
