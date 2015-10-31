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

//Scene* OverLayer::createScene()
//{
//    auto scene = Scene::create();
//    auto layer = OverLayer::create();
//    scene->addChild(layer);
//    return scene;
//}

bool OverLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    auto rootNode = CSLoader::createNode("over/over.csb");
    addChild(rootNode);
    
    int bestScore = db->getIntegerForKey(key_best_score.c_str(), 0);
    int rating;
    if(bestScore < score) {
        rating = 3;
        bestScore = score;
        db->setIntegerForKey(key_best_score.c_str(), score);
        db->flush();
    }
    else if (score > bestScore*2.0/3){
        rating = 2;
    }
    else if (score > bestScore*1.0/3) {
        rating = 1;
    }
    else rating = 0;
    
    Text* scoreTxt = dynamic_cast<Text*>(rootNode->getChildByName("Text_score"));
    scoreTxt->setString(StringUtils::format("%d", score));
    
    Text* bestScoreTxt = dynamic_cast<Text*>(rootNode->getChildByName("Text_bestscore"));
    bestScoreTxt->setString(StringUtils::format("%d", bestScore));
    
//    Sprite* ratingSprite = dynamic_cast<Sprite*>(rootNode->getChildByName("sprite_ratings"));
//    Texture2D* newRatingTexture;
//    if(rating != 0) {
//        newRatingTexture = Director::getInstance()->getTextureCache()->addImage(res_ratings[rating]);
//        ratingSprite->setTexture(newRatingTexture);
//    }
    
    Button* anotherBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_newgame"));
    Button* returnBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_back"));
    Button* shareBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_share"));
    anotherBtn->addTouchEventListener(CC_CALLBACK_2(OverLayer::anotherCallback, this));
    returnBtn->addTouchEventListener(CC_CALLBACK_2(OverLayer::returnCallback, this));
    shareBtn->addTouchEventListener(CC_CALLBACK_2(OverLayer::shareCallback, this));
    
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(OverLayer::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
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
/**
 * call when share button is clicked.
 */

void OverLayer::shareCallback(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
    if (type == Widget::TouchEventType::ENDED) {
        Dictionary *share_content = Dictionary::create();
        share_content -> setObject(String::create("。。我是一只软软的Nano。。。欢迎来到软蠢萌的世界"), "content");
        share_content -> setObject(String::create(deadScreen.c_str()), "image");
        share_content -> setObject(String::create("Nano Trip"), "title");
        share_content -> setObject(String::create("来自某只Nano的分享"), "description");
        share_content -> setObject(String::create("http://www.1-xing.com"), "url");
        share_content -> setObject(String::createWithFormat("%d", C2DXContentTypeNews), "type");
        share_content -> setObject(String::create("http://www.1-xing.com"), "siteUrl");
        share_content -> setObject(String::create("1-xing"), "site");
        share_content -> setObject(String::create("extInfo"), "extInfo");
        
        C2DXShareSDK::showShareMenu(NULL, share_content, Vec2(100, 100), C2DXMenuArrowDirectionLeft, shareResultHandler);
    }
}
void OverLayer::shareResultHandler(cn::sharesdk::C2DXResponseState state, cn::sharesdk::C2DXPlatType platType, CCDictionary *shareInfo, CCDictionary *error) {
    switch (state) {
        case cn::sharesdk::C2DXResponseStateSuccess:
            CCLOG("分享成功");
            break;
        case cn::sharesdk::C2DXResponseStateFail:
            CCLOG("分享失败");
        default:
            break;
    }
}
void OverLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    switch(keyCode)
    {
            //监听返回键
        case EventKeyboard::KeyCode::KEY_ESCAPE:
            returnCallback(NULL, Widget::TouchEventType::ENDED);
            break;
            //监听menu键
        case EventKeyboard::KeyCode::KEY_MENU:
            break;
    }
}