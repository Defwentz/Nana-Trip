//
//  OverLayer.cpp
//  nanatrip
//
//  Created by Macbook Air on 7/29/15.
//
//

#include "OverLayer.h"
#include "StartLayer.h"
#include "GameLayer.h"

USING_NS_CC;
using namespace cocos2d::ui;

Scene* OverLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = OverLayer::create();
    scene->addChild(layer);
    return scene;
}

bool OverLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    auto rootNode = CSLoader::createNode("result/result.csb");
    addChild(rootNode);
    
    Button* anotherBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_newgame"));
    Button* returnBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_back"));
    anotherBtn->addTouchEventListener(CC_CALLBACK_2(OverLayer::anotherCallback, this));
    returnBtn->addTouchEventListener(CC_CALLBACK_2(OverLayer::returnCallback, this));
    
    return true;
}

// another round
void OverLayer::anotherCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
    Director::getInstance()->replaceScene(GameLayer::createScene());
}
// return to menu
void OverLayer::returnCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
    Director::getInstance()->replaceScene(StartLayer::createScene());
}