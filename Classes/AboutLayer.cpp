//
//  AboutLayer.cpp
//  nanatrip
//
//  Created by Macbook Air on 9/11/15.
//
//

#include "AboutLayer.h"
#include "StartLayer.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace cocos2d::ui;

Scene* AboutLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = AboutLayer::create();
    scene->addChild(layer);
    return scene;
}

bool AboutLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("about/about.csb");
    addChild(rootNode);
    
    Button* backBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_back"));
    backBtn->addTouchEventListener(CC_CALLBACK_2(AboutLayer::backCallback, this));
    
    return true;
}

void AboutLayer::backCallback(Ref* sender, Widget::TouchEventType type)
{
    if (type == Widget::TouchEventType::ENDED)
    {
        Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
        Director::getInstance()->replaceScene(StartLayer::createScene());
    }
}