//
//  StartLayer.cpp
//  nanatrip
//
//  Created by Macbook Air on 7/30/15.
//
//

#include "StartLayer.h"
#include "GameLayer.h"
#include "AboutLayer.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace cocos2d::ui;

Scene* StartLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = StartLayer::create();
    scene->addChild(layer);
    return scene;
}

bool StartLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("start/start.csb");
    addChild(rootNode);
    
    startBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_start"));
//    storeBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_store"));
//    aboutBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_about"));
    startBtn->addTouchEventListener(CC_CALLBACK_2(StartLayer::startCallback, this));
//    storeBtn->addTouchEventListener(CC_CALLBACK_2(StartLayer::storeCallback, this));
//    aboutBtn->addTouchEventListener(CC_CALLBACK_2(StartLayer::aboutCallback, this));
    
    return true;
}

void StartLayer::startCallback(Ref* sender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
        Director::getInstance()->replaceScene(GameLayer::createScene());
    }
}
//void StartLayer::storeCallback(Ref* sender, Widget::TouchEventType type)
//{
//    if (type == Widget::TouchEventType::ENDED)
//    {
//        //Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
//        //CCDirector::getInstance()->replaceScene(GameLayer::createScene());
//    }
//}
//void StartLayer::aboutCallback(Ref* sender, Widget::TouchEventType type)
//{
//    if (type == Widget::TouchEventType::ENDED)
//    {
//        Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
//        Director::getInstance()->replaceScene(AboutLayer::createScene());
//    }
//}