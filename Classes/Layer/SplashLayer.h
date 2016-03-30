//
//  SplashLayer.hpp
//  nana
//
//  Created by Macbook Air on 2/26/16.
//
//

#ifndef SplashLayer_hpp
#define SplashLayer_hpp
#include "NanaTrip.h"

class SplashLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(SplashLayer);
    void gotoStart(float dt);
};

#endif /* SplashLayer_hpp */
