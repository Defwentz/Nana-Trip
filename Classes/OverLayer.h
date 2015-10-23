//
//  OverLayer.h
//  nanatrip
//
//  Created by Macbook Air on 7/29/15.
//
//

#ifndef __nanatrip__OverLayer__
#define __nanatrip__OverLayer__

#include "NanaTrip.h"

//using namespace cn::sharesdk;

class OverLayer : public cocos2d::Layer
{

public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(OverLayer);
    
    void anotherCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void returnCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void shareCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
        //static void shareResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *shareInfo, CCDictionary *error);
};

#endif /* defined(__nanatrip__OverLayer__) */
