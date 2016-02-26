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

void Randomer::updateOdds(int item, int newOdds)
{
    for(int i = 0; i < _item.size(); i++)
    {
        if(_item[i] == item) {
            odds_mask -= _odds[i];
            _odds[i] = newOdds;
            odds_mask += newOdds;
            return;
        }
    }
}

void Randomer::rmvItem(int item) {
    std::vector<int>::iterator it = _item.begin();
    while(it != _item.end()) {
        if (*it == item) {
            _item.erase(it);
            return;
        }
    }
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