//
//  Randomer.cpp
//  nanatrip
//
//  Created by Macbook Air on 7/26/15.
//
//

#include "Randomer.h"

Randomer::Randomer()
{
    odds_mask = 0;
}

void Randomer::add(int item, int odds)
{
    this->_item.push_back(item);
    this->_odds.push_back(odds);
    odds_mask += odds;
}

int Randomer::getRandomItem()
{
    //if(odds_mask == 0)return 0;
    // didn't consider when odds_mask might be 0
    int r = rand() % odds_mask;
    for(int i = 0; i < _item.size(); i++)
    {
        if(r < _odds[i])
            return _item[i];
        else
            r -= _odds[i];
    }
    
    return 0;
}