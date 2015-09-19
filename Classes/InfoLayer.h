//
//  InfoLayer.h
//  nanatrip
//
//  Created by Macbook Air on 7/27/15.
//
//

#ifndef __nanatrip__InfoLayer__
#define __nanatrip__InfoLayer__

#include "NanaTrip.h"

class InfoLayer : public cocos2d::Layer
{
    int old_pos_score;
    cocos2d::Label *scoreLabel;
    cocos2d::ui::Button *pauseBtn;
    cocos2d::ui::Button *soundBtn;
public:
    virtual bool init();
    CREATE_FUNC(InfoLayer);
    ~InfoLayer();
    void reset();
    
    void update();
    void pauseCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void soundCallback(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void defaultCallback(cocos2d::Ref *pSender);
};

#endif /* defined(__nanatrip__InfoLayer__) */
