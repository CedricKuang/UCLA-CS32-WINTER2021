/*#include <string>
#ifndef NEWSEQUENCE_H
#define NEWSEQUENCE_H

using ItemType = unsigned long;
const int DEFAULT_MAX_ITEMS = 150;

class Sequence
{
private:
    ItemType *m_sq;
    int m_nsq;
    int m_maxn;
    //supplementary functions
    ItemType visit(int pos) const;
    void override(int pos, const ItemType& value);
    void setsize(int size);
    int getmaxn() const;
public:
    Sequence(int size = DEFAULT_MAX_ITEMS);
    ~Sequence();
    Sequence(const Sequence& sq);
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
    Sequence& operator=(const Sequence& sq);
};


#endif*/
