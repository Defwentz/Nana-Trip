//
//  SplashLayer.cpp
//  nana
//
//  Created by Macbook Air on 2/26/16.
//
//

#include "SplashLayer.h"
#include "StartLayer.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace cocos2d::ui;

Scene* SplashLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = SplashLayer::create();
    scene->addChild(layer);
    return scene;
}

bool SplashLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("splash/splash.csb");
    addChild(rootNode);
    scheduleOnce(schedule_selector(SplashLayer::gotoStart), 1.5f);
    
    return true;
}

void SplashLayer::gotoStart(float dt) {
    Director::getInstance()->replaceScene(StartLayer::createScene());
}
