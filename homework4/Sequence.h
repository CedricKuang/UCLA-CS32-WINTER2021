#ifndef SEQUENCE_INCLUDED
#define SEQUENCE_INCLUDED

template <typename ItemType>
class Sequence
{
public:
    Sequence();          
    bool empty() const { return m_size == 0; }
    int size() const { return m_size; }
    int insert(int pos, const ItemType& value);
    int insert(const ItemType& value);
    bool erase(int pos);
    int remove(const ItemType& value);
    bool get(int pos, ItemType& value) const;
    bool set(int pos, const ItemType& value);
    int find(const ItemType& value) const;
    void swap(Sequence& other);
    ~Sequence();
    Sequence(const Sequence& other);
    Sequence& operator=(const Sequence& rhs);

private:
    struct Node
    {
        ItemType m_value;
        Node* m_next;
        Node* m_prev;
    };

    Node* m_head;
    int   m_size;

    void createEmpty();
    void insertBefore(Node* p, const ItemType& value);
    Node* doErase(Node* p);
    Node* nodeAtPos(int pos) const;
};

// Declarations of non-member functions
template <typename ItemType>
int subsequence(const Sequence<ItemType>& seq1, const Sequence<ItemType>& seq2);

template <typename ItemType>
void interleave(const Sequence<ItemType>& seq1, const Sequence<ItemType>& seq2, Sequence<ItemType>& result);


template<typename ItemType>
Sequence<ItemType>::Sequence()
{
    createEmpty();
}

template<typename ItemType>
Sequence<ItemType>::~Sequence()
{
    while (m_head->m_next != m_head)
        doErase(m_head->m_next);
    delete m_head;
}

template<typename ItemType>
Sequence<ItemType>::Sequence(const Sequence& other)
{
    createEmpty();
    for (Node* p = other.m_head->m_next; p != other.m_head; p = p->m_next)
        insertBefore(m_head, p->m_value);
}

template<typename ItemType>
Sequence<ItemType>& Sequence<ItemType>::operator=(const Sequence<ItemType>& rhs)
{
    if (this != &rhs)
    {
        Sequence temp(rhs);
        swap(temp);
    }
    return *this;
}

template<typename ItemType>
int Sequence<ItemType>::insert(int pos, const ItemType& value)
{
    if (pos < 0 || pos > m_size)
        return -1;

    Node* p = nodeAtPos(pos);
    insertBefore(p, value);

    return pos;
}

template<typename ItemType>
int Sequence<ItemType>::insert(const ItemType& value)
{
    Node* p;
    int pos;
    for (p = m_head->m_next, pos = 0; p != m_head &&
        value > p->m_value; p = p->m_next, pos++)
        ;

    insertBefore(p, value);
    return pos;
}

template<typename ItemType>
bool Sequence<ItemType>::erase(int pos)
{
    if (pos < 0 || pos >= m_size)
        return false;

    Node* p = nodeAtPos(pos);
    doErase(p);

    return true;
}

template<typename ItemType>
int Sequence<ItemType>::remove(const ItemType& value)
{
    int count = 0;

    Node* p = m_head->m_next;
    while (p != m_head)
    {
        if (p->m_value == value)
        {
            count++;
            p = doErase(p); 
        }
        else
            p = p->m_next; 
    }
    return count;
}

template<typename ItemType>
bool Sequence<ItemType>::get(int pos, ItemType& value) const
{
    if (pos < 0 || pos >= m_size)
        return false;

    Node* p = nodeAtPos(pos);
    value = p->m_value;

    return true;
}

template<typename ItemType>
bool Sequence<ItemType>::set(int pos, const ItemType& value)
{
    if (pos < 0 || pos >= m_size)
        return false;

    Node* p = nodeAtPos(pos);
    p->m_value = value;

    return true;
}

template<typename ItemType>
int Sequence<ItemType>::find(const ItemType& value) const
{
    int pos = 0;
    Node* p = m_head->m_next;
    for (; p != m_head && p->m_value != value; p = p->m_next, pos++)
        ;
    return p == m_head ? -1 : pos;
}

template<typename ItemType>
void Sequence<ItemType>::swap(Sequence<ItemType>& other)
{
    Node* p = other.m_head;
    other.m_head = m_head;
    m_head = p;

    int s = other.m_size;
    other.m_size = m_size;
    m_size = s;
}

template<typename ItemType>
void Sequence<ItemType>::createEmpty()
{
    m_size = 0;
    m_head = new Node;
    m_head->m_next = m_head;
    m_head->m_prev = m_head;
}

template<typename ItemType>
void Sequence<ItemType>::insertBefore(Node* p, const ItemType& value)
{
    Node* newp = new Node;
    newp->m_value = value;

    newp->m_prev = p->m_prev;
    newp->m_next = p;
    newp->m_prev->m_next = newp;
    newp->m_next->m_prev = newp;

    m_size++;
}

template<typename ItemType>
typename Sequence<ItemType>::Node* Sequence<ItemType>::doErase(Node* p)
{
    Node* pnext = p->m_next;

    p->m_prev->m_next = p->m_next;
    p->m_next->m_prev = p->m_prev;
    delete p;

    m_size--;

    return pnext;
}

template<typename ItemType>
typename Sequence<ItemType>::Node* Sequence<ItemType>::nodeAtPos(int pos) const
{
    Node* p;

    if (pos <= m_size / 2)  // closer to head
    {
        p = m_head->m_next;
        for (int k = 0; k != pos; k++)
            p = p->m_next;
    }
    else  // closer to tail
    {
        p = m_head;
        for (int k = m_size; k != pos; k--)
            p = p->m_prev;
    }

    return p;
}

template<typename ItemType>
int subsequence(const Sequence<ItemType>& seq1, const Sequence<ItemType>& seq2)
{
    if (seq2.empty())
        return -1;

    for (int pos = 0; pos <= seq1.size() - seq2.size(); pos++)
    {
        bool allMatched = true;

        for (int k = 0; k < seq2.size(); k++)
        {
            ItemType v1;
            ItemType v2;
            seq1.get(pos + k, v1);
            seq2.get(k, v2);
            if (v1 != v2)
            {
                allMatched = false;
                break;
            }
        }

        if (allMatched)
            return pos;
    }

    return -1;
}

template<typename ItemType>
void interleave(const Sequence<ItemType>& seq1, const Sequence<ItemType>& seq2, Sequence<ItemType>& result)
{

    Sequence<ItemType> res;

    int n1 = seq1.size();
    int n2 = seq2.size();
    int nmin = (n1 < n2 ? n1 : n2);
    int resultPos = 0;
    for (int k = 0; k < nmin; k++)
    {
        ItemType v;
        seq1.get(k, v);
        res.insert(resultPos, v);
        resultPos++;
        seq2.get(k, v);
        res.insert(resultPos, v);
        resultPos++;
    }

    const Sequence<ItemType>& s = (n1 > nmin ? seq1 : seq2);
    int n = (n1 > nmin ? n1 : n2);
    for (int k = nmin; k < n; k++)
    {
        ItemType v;
        s.get(k, v);
        res.insert(resultPos, v);
        resultPos++;
    }

    result.swap(res);
}

#endif
