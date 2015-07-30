//
//  StartLayer.cpp
//  nanatrip
//
//  Created by Macbook Air on 7/30/15.
//
//

#include "StartLayer.h"

USING_NS_CC;

using namespace cocostudio::timeline;

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
    
    auto rootNode = CSLoader::createNode("about/about.csb");
    
    addChild(rootNode);
    
    return true;
}