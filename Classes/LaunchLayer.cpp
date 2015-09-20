//
//  LaunchLayer.cpp
//  nanatrip
//
//  Created by Macbook Air on 9/19/15.
//
//

#include "LaunchLayer.h"
#include "StartLayer.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace cocos2d::ui;

Scene* LaunchLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = LaunchLayer::create();
    scene->addChild(layer);
    return scene;
}

bool LaunchLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    auto bgSprite = Sprite::create("splash.jpg");
    bgSprite->setPosition(Vec2(winMidX, winMidY));
    bgSprite->setScale(winSiz.width/640, winSiz.height/1136);
    this->addChild(bgSprite);
    scheduleOnce(schedule_selector(LaunchLayer::launch),1.5f);
    return true;
}

void LaunchLayer::launch(float dt) {
    Director::getInstance()->replaceScene(StartLayer::createScene());
}