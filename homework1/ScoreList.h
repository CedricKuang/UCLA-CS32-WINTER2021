/*#include "Sequence.h"
#include <limits>

#ifndef SCORELIST_H
#define SCORELIST_H

const unsigned long NO_SCORE = std::numeric_limits<unsigned long>::max();

class ScoreList
{
public:
    ScoreList();      
    bool add(ItemType score);
    bool remove(ItemType score);
    int size() const;  
    ItemType minimum() const;
    ItemType maximum() const;
private:
    Sequence m_sequence;
};

#endif*/