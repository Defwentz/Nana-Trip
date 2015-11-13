//
//  JavaOCer.cpp
//  nana
//
//  Created by Macbook Air on 11/12/15.
//
//

#include "JavaOCer.hpp"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <platform/android/jni/JniHelper.h>
#include <jni.h>

bool getJavaMethod(JniMethodInfo &mi, const char *methodName, const char *paramCode) {
    return JniHelper::getStaticMethodInfo(mi, "org/cocos2dx/mz/AppActivity", methodName, paramCode);
}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "ABGameKitHelper.h"
#endif


void JavaOCer::init() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // 连接GameCenter
    [ABGameKitHelper sharedHelper];
#endif
}

bool JavaOCer::showMsg(char *msg) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JniMethodInfo mi;
    bool isHave = getJavaMethod(mi, "makeToast", "(Ljava/lang/String;)V");
    if (!isHave) {
        return false;
    }
    jstring msg = mi.env->NewStringUTF(msg);
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, msg);
    mi.env->DeleteLocalRef(mi.classID);
    return true;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [ABGameKitHelper MessageBox:[NSString stringWithUTF8String:msg]
                          title:@"弹窗咯"];
    return true;
#endif
}

void JavaOCer::reportScore4Leaderboard(int score, char *leaderboardId) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    showMsg("嗯。。假装已经在网上记录了最高分");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[ABGameKitHelper sharedHelper]
     reportScore:score
     forLeaderboard:[NSString stringWithUTF8String:leaderboardId]];
#endif
}

void JavaOCer::showLeaderboard(char *leaderboardId) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    showMsg("先放着。。不着急");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[ABGameKitHelper sharedHelper]
     showLeaderboard:[NSString stringWithUTF8String:leaderboardId]];
#endif
}

void JavaOCer::reportAchievement(float progress, char *achievementId) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    showMsg("嗯。。假装report这个成就了");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[ABGameKitHelper sharedHelper]
     reportAchievement:[NSString stringWithUTF8String:achievementId]
     percentComplete:progress];
#endif
}

void JavaOCer::showAchievementNotification(char *title, char *msg, char *achievementId) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    showMsg("成就banner!!");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [[ABGameKitHelper sharedHelper]
     showNotification:[NSString stringWithUTF8String:title]
     message:[NSString stringWithUTF8String:msg]
     identifier:[NSString stringWithUTF8String:achievementId]];
#endif
}