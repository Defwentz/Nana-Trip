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
    
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    CREATE_FUNC(StartLayer);
    
    //void update(float dt);
};

#endif /* defined(__nanatrip__StartLayer__) */
