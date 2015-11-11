//
//  InterestingLayer.c
//  nana
//
//  Created by Macbook Air on 11/10/15.
//
//

#include "InterestingLayer.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "ABGameKitHelper.h"
#endif

bool InterestingLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[ABGameKitHelper sharedHelper] reportAchievement:@"nana_leaving_game_world" percentComplete:100.0f];
    [[ABGameKitHelper sharedHelper] showNotification:@"离开游戏世界" message:@"nano坐着小黄球离开了游戏世界!" identifier:@"nana_leaving_game_world"];
#endif
    return true;
}