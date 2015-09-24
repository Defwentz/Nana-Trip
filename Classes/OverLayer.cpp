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
    
    Sprite* ratingSprite = dynamic_cast<Sprite*>(rootNode->getChildByName("sprite_ratings"));
    Texture2D* newRatingTexture;
    if(rating != 0) {
        newRatingTexture = Director::getInstance()->getTextureCache()->addImage(res_ratings[rating]);
        ratingSprite->setTexture(newRatingTexture);
    }
    
    Button* anotherBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_newgame"));
    Button* returnBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_back"));
    Button* shareBtn = dynamic_cast<Button*>(rootNode->getChildByName("button_share"));
    anotherBtn->addTouchEventListener(CC_CALLBACK_2(OverLayer::anotherCallback, this));
    returnBtn->addTouchEventListener(CC_CALLBACK_2(OverLayer::returnCallback, this));
    shareBtn->addTouchEventListener(CC_CALLBACK_2(OverLayer::shareCallback, this));
    
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
        //content -> setObject(String::create("http://sharesdk.cn"), "url");
        share_content -> setObject(String::createWithFormat("%d", C2DXContentTypeNews), "type");
        //content -> setObject(String::create("http://sharesdk.cn"), "siteUrl");
        //content -> setObject(String::create("ShareSDK"), "site");
        //content -> setObject(String::create("http://mp3.mwap8.com/destdir/Music/2009/20090601/ZuiXuanMinZuFeng20090601119.mp3"), "musicUrl");
        share_content -> setObject(String::create("extInfo"), "extInfo");
        C2DXShareSDK::showShareMenu(NULL, share_content, CCPointMake(100, 100), C2DXMenuArrowDirectionLeft, shareResultHandler);
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