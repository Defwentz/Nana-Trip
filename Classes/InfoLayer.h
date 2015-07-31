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
    void pauseBtnTouched(Object *pSender, cocos2d::ui::TouchEventType type);
public:
    virtual bool init();
    CREATE_FUNC(InfoLayer);
    
    void update(float dt);
    void reset();
};

#endif /* defined(__nanatrip__InfoLayer__) */
