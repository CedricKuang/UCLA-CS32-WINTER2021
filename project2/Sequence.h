#include <string>
#ifndef SEQUENCE_H
#define SEQUENCE_H

/*#include "IntWrapper.h"      // TODO: remove this line after testing
using ItemType = IntWrapper; // TODO: replace this line after testing*/
using ItemType = std::string;


class Sequence
{
public:
    Sequence(); 
    ~Sequence();    //D'tor
    Sequence(const Sequence& s);    //copy constructor
    Sequence& operator=(const Sequence& s);      //assignment operator
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
    //void dump() const;
private:
    struct Node         //the double linked list node, which contains two pointers
    {
        ItemType value;
        Node* next;     //point to next node
        Node* prev;     //point to previous node
    };
    Node* head;     //head pointer
    Node* tail;     //tail pointer
    int m_size;     //number of nodes in the sequence
    //auxillary functions
    void addtofront(const ItemType& value);
    void additem(int pos, const ItemType& value);
    void addtorear(const ItemType& value);
};

int subsequence(const Sequence& seq1, const Sequence& seq2);
void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);


#endif
