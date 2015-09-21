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

    auto rootNode = CSLoader::createNode("over/over.csb");
    addChild(rootNode);
    
    UserDefault *db = UserDefault::getInstance();
    int bestScore = db->getIntegerForKey("best_score", 0);
    if(bestScore < score) {
        bestScore = score;
        db->setIntegerForKey("best_score", score);
        db->flush();
    }
    
    Text* scoreTxt = dynamic_cast<Text*>(rootNode->getChildByName("Text_score"));
    scoreTxt->setString(StringUtils::format("%d", score));
    
    Text* bestScoreTxt = dynamic_cast<Text*>(rootNode->getChildByName("Text_bestscore"));
    bestScoreTxt->setString(StringUtils::format("%d", bestScore));
    
    Button* anotherBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_newgame"));
    Button* returnBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_back"));
    anotherBtn->addTouchEventListener(CC_CALLBACK_2(OverLayer::anotherCallback, this));
    returnBtn->addTouchEventListener(CC_CALLBACK_2(OverLayer::returnCallback, this));
    
    return true;
}

/**
 * call when new-game button is clicked.
 */
void OverLayer::anotherCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED) {
        Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
        Director::getInstance()->replaceScene(GameLayer::createScene());
    }
}
/**
 * call when return-to-menu button is clicked.
 */
void OverLayer::returnCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED) {
        Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
        Director::getInstance()->replaceScene(StartLayer::createScene());
    }
}