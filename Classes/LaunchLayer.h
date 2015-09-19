//
//  LaunchLayer.h
//  nanatrip
//
//  Created by Macbook Air on 9/19/15.
//
//

#ifndef __nanatrip__LaunchLayer__
#define __nanatrip__LaunchLayer__

#include "NanaTrip.h"

class LaunchLayer : public cocos2d::Layer
{
    
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(LaunchLayer);
    
    void launch(float dt);
    
};

#endif /* defined(__nanatrip__LaunchLayer__) */
