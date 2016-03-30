//
//  JavaOCer.hpp
//  nana
//
//  Created by Macbook Air on 11/12/15.
//
//

#ifndef JavaOCer_hpp
#define JavaOCer_hpp

class JavaOCer {
public:
    static void leavingStartLayer();
    
    static void loadInterAd();
    static void showInterAd();
    
    // achievements and things
    static void init();
    static bool showMsg(char *msg);
    static void showLeaderboard(char *leaderboardId);
    static void reportScore4Leaderboard(int score, char *leaderboardId);
    static void reportAchievement(float progress, char *achievementId);
    static void showAchievementNotification(char *title, char *msg, char *achievementId);
};

#endif /* JavaOCer_hpp */
