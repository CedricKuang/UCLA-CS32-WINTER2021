#include <string>
#ifndef SEQUENCE_H
#define SEQUENCE_H

using ItemType = unsigned long;
const int DEFAULT_MAX_ITEMS = 150;

class Sequence
{
private:
    ItemType m_sq[DEFAULT_MAX_ITEMS];
    int m_nsq;
    //supplementary functions used only in swap().
    ItemType visit(int pos) const;
    void override(int pos, const ItemType& value);
    void setsize(int size);
public:
    Sequence();    
    bool empty() const; 
    int size() const;    
    int insert(int pos, const ItemType& value);
    int insert(const ItemType& value);
    bool erase(int pos);
    int remove(const ItemType& value);
    bool get(int pos, ItemType& value) const;
    bool set(int pos, const ItemType& value);
    int find(const ItemType& value) const;
    void swap(Sequence& other);
};



#endif
