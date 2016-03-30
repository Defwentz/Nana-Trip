//
//  InterestingLayer.c
//  nana
//
//  Created by Macbook Air on 11/10/15.
//
//

#include "InterestingLayer.h"

bool InterestingLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    JavaOCer::reportAchievement(100.f, "nana_leaving_game_world");
    JavaOCer::showAchievementNotification("离开游戏世界", "nano坐着小黄球离开了游戏世界!", "nana_leaving_game_world");
    return true;
}