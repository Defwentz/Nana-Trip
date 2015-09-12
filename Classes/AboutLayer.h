//
//  AboutLayer.h
//  nanatrip
//
//  Created by Macbook Air on 9/11/15.
//
//

#ifndef __nanatrip__AboutLayer__
#define __nanatrip__AboutLayer__

#include "NanaTrip.h"

class AboutLayer : public cocos2d::Layer
{
    
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(AboutLayer);
    
    void backCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
};

#endif /* defined(__nanatrip__AboutLayer__) */
