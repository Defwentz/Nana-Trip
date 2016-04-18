//
//  JavaOCer.cpp
//  nana
//
//  Created by Macbook Air on 11/12/15.
//
//

#include "JavaOCer.hpp"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <cocos2d.h>
#include <platform/android/jni/JniHelper.h>
#include <jni.h>

USING_NS_CC;

bool getJavaMethod(JniMethodInfo &mi, const char *methodName, const char *paramCode) {
    return JniHelper::getStaticMethodInfo(mi, "org/cocos2dx/mz/AppActivity", methodName, paramCode);
}

void JavaOCer::leavingStartLayer() {
    JniMethodInfo t;
    if(getJavaMethod(t, "hidebar", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

void JavaOCer::loadInterAd() {
//    JniMethodInfo t;
//    if(getJavaMethod(t, "loadBaiduAd", "()V")) {
//        t.env->CallStaticVoidMethod(t.classID, t.methodID);
//        t.env->DeleteLocalRef(t.classID);
//    }
}
void JavaOCer::showInterAd() {
//    JniMethodInfo t;
//    if(getJavaMethod(t, "showBaiduAd", "()V")) {
//        t.env->CallStaticVoidMethod(t.classID, t.methodID);
//        t.env->DeleteLocalRef(t.classID);
//    }
}

void JavaOCer::init() {
    JniMethodInfo t;
    if(getJavaMethod(t, "showbar", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

bool JavaOCer::showMsg(char *msg) {
    JniMethodInfo mi;
    bool isHave = getJavaMethod(mi, "makeToast", "(Ljava/lang/String;)V");
    if (!isHave) {
        return false;
    }
    jstring _msg = mi.env->NewStringUTF(msg);
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, _msg);
    mi.env->DeleteLocalRef(mi.classID);
    return true;
}

void JavaOCer::reportScore4Leaderboard(int score, char *leaderboardId) {
    JniMethodInfo mi;
    bool isHave = getJavaMethod(mi, "reportScore", "(I)V");
    if (!isHave) {
        return;
    }
    mi.env->CallStaticVoidMethod(mi.classID, mi.methodID, score);
    mi.env->DeleteLocalRef(mi.classID);
    return;
}

void JavaOCer::showLeaderboard(char *leaderboardId) {
    JniMethodInfo t;
    if(getJavaMethod(t, "showLeaderboard", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

void JavaOCer::reportAchievement(float progress, char *achievementId) {
    showMsg(achievementId);//"嗯。。假装report这个成就了");
}

void JavaOCer::showAchievementNotification(char *title, char *msg, char *achievementId) {
    log("showAchievementNotification: %s", achievementId);
    showMsg(msg);
}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "ABGameKitHelper.h"

void JavaOCer::leavingStartLayer() {
}

void JavaOCer::loadInterAd() {
    
}
void JavaOCer::showInterAd() {
    //showMsg("check out this ad...");
}

void JavaOCer::init() {
    // 连接GameCenter
    [ABGameKitHelper sharedHelper];
}
bool JavaOCer::showMsg(char *msg) {
    [ABGameKitHelper MessageBox:[NSString stringWithUTF8String:msg]
                          title:@"弹窗咯"];
    return true;
}

void JavaOCer::reportScore4Leaderboard(int score, char *leaderboardId) {
    [[ABGameKitHelper sharedHelper]
     reportScore:score
     forLeaderboard:[NSString stringWithUTF8String:leaderboardId]];
}

void JavaOCer::showLeaderboard(char *leaderboardId) {
    [[ABGameKitHelper sharedHelper]
     showLeaderboard:[NSString stringWithUTF8String:leaderboardId]];
}

void JavaOCer::reportAchievement(float progress, char *achievementId) {
    [[ABGameKitHelper sharedHelper]
     reportAchievement:[NSString stringWithUTF8String:achievementId]
     percentComplete:progress];
}

void JavaOCer::showAchievementNotification(char *title, char *msg, char *achievementId) {
    [[ABGameKitHelper sharedHelper]
     showNotification:[NSString stringWithUTF8String:title]
     message:[NSString stringWithUTF8String:msg]
     identifier:[NSString stringWithUTF8String:achievementId]];
}
#endif
