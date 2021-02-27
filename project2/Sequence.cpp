#include "Sequence.h"

Sequence::Sequence()
{
	head = nullptr;
	tail = nullptr;
	m_size = 0;
}

Sequence::~Sequence()
{
	Node* p = head;
	while (p != nullptr)
	{
		Node* n = p->next;
		delete p;
		p = n;
	}
}

Sequence::Sequence(const Sequence& s)
{
	head = nullptr;
	tail = nullptr;
	m_size = s.m_size;
	if (s.empty())   //if s is empty, do nothing
		return;

	Node* p = s.head;    //setting the head pointer for this sequence specially
	Node* n = new Node;
	n->value = p->value;
	n->prev = nullptr;
	n->next = nullptr;
	head = n;

	Node* temp = n;    //this temp pointer points to the node before the new being constructed node 
	p = p->next;
	while (p != nullptr)    //go through every node in sequence s, and copt them into this sequence
	{
		Node* n = new Node;
		n->value = p->value;
		temp->next = n;
		n->prev = temp;
		temp = temp->next;
		p = p->next;
	}
	tail = temp;    //set the tail pointer
	tail->next = nullptr;
}

Sequence& Sequence::operator=(const Sequence& s)
{
	if (this == &s)
		return *this;

	Node* m = head;        //deleting the old content in this sequence
	while (m != nullptr)
	{
		Node* n = m->next;
		delete m;
		m = n;
	}
	head = nullptr;
	tail = nullptr;
	m_size = s.m_size;
	if (s.empty())   //if s is empty, do nothing
		return *this;
	//following is the same as copy constructor
	Node* p = s.head;    //setting the head pointer for this sequence specially
	Node* n = new Node;
	n->value = p->value;
	n->prev = nullptr;
	n->next = nullptr;
	head = n;

	Node* temp = n;    //this temp pointer points to the node before the new being constructed node 
	p = p->next;
	while (p != nullptr)    //go through every node in sequence s, and copy them into this sequence
	{
		Node* n = new Node;
		n->value = p->value;
		temp->next = n;
		n->prev = temp;
		temp = temp->next;
		p = p->next;
	}
	tail = temp;    //set the tail pointer
	tail->next = nullptr;
	return *this;
}

bool Sequence::empty() const
{
	if (head == nullptr && tail == nullptr)
		return true;
	else
		return false;
}

int Sequence::size() const
{
	return m_size;
}

int Sequence::insert(int pos, const ItemType& value)
{
	if (pos < 0 || pos > m_size)
		return -1;
    
	if (pos == 0)                 //if add at the very front, head points to n
		addtofront(value);
	else if (pos == m_size)   //if add to the rear
		addtorear(value);
	else                          //add to the middle of somewhere
		additem(pos, value);
	m_size++;
	return pos;
}

int Sequence::insert(const ItemType& value)
{
	Node* p = head;
	int pos = 0;
	while (p != nullptr && p->value < value)  //find the position of the value
	{
		p = p->next;
		pos++;
	}
	return insert(pos, value);
}

bool Sequence::erase(int pos)
{
	if (pos < 0 || pos >= m_size)
		return false;
	
	if (pos == 0)        //if delete the front
	{
		Node* p = head;
		head = p->next;
		if (head == nullptr)
			tail = nullptr;
		else
			head->prev = nullptr;
		delete p;
	}
	else if (pos == m_size - 1)   //if delete the last node
	{
		Node* p = tail;
		p->prev->next = nullptr;
		tail = p->prev;
		delete p;
	}
	else   //if delete the middle one
	{
		Node* p = head;
		for (int i = 0; i < pos; i++)    //find the node at that position
			p = p->next;
		p->prev->next = p->next;
		p->next->prev = p->prev;
		delete p;
	}
	m_size--;
	return true;
}

int Sequence::remove(const ItemType& value)
{
	int count = 0;
	Node* p = head;
	for (int i = 0; i < m_size; i++)     //go thorugh every node in the sequence
	{
		if (p == nullptr)
			break;
		Node* temp = p->next;
		if (p->value == value)       //if the value is equal, erase the node at that posoition
		{
			erase(i);
			count++;
		}
		p = temp;    //at this time, p points to nothing since the original node is deleted. Point p to p->next (stored in temp before)
		i--;         //decrease i by 1 since m_size has been decreased by one
	}
	return count;
}

bool Sequence::get(int pos, ItemType& value) const
{
	if (pos < 0 || pos >= m_size)
		return false;

	if (pos == 0)       //if get the first node
		value = head->value;
	else if (pos == m_size - 1)    //if get the last node
		value = tail->value;
	else     //if get the node in the middle
	{
		Node* p = head;
		for (int i = 0; i < pos; i++)
			p = p->next;
		value = p->value;
	}
	return true;
}

bool Sequence::set(int pos, const ItemType& value)
{
	if (pos < 0 || pos >= m_size)
		return false;

	if (pos == 0)       //if set the first node
		head->value = value;
	else if (pos == m_size - 1)    //if set the last node
		tail->value = value;
	else    //if set the node in the middle
	{
		Node* p = head;
		for (int i = 0; i < pos; i++)
			p = p->next;
		p->value = value;
	}
	return true;
}

int Sequence::find(const ItemType& value) const
{
	int p;
	Node* n = head;
	for (p = 0; p < m_size; p++)   //go thorugh each node in the sequence
	{
		if (n->value == value)     //if the node's value is equal to value, return p
			return p;
		n = n->next;
	}
	p = -1;
	return p;
}

void Sequence::swap(Sequence& other)
{
	Sequence temp(other);
	other = *this;
	*this = temp;
}

/*#include <iostream>
void Sequence::dump() const
{
	Node* p = head;
	while (p != nullptr)
	{
	  std::cerr << p->value << " ";
	  p = p->next;
	}
}*/

void Sequence::addtofront(const ItemType& value)
{
	Node* n = new Node;
	n->value = value;
	n->next = head;
	n->prev = nullptr;
	if (head != nullptr)
		head->prev = n;
	head = n;
	if (m_size == 0)     //tail also points to n if is empty
		tail = n;
}

void Sequence::addtorear(const ItemType& value)
{
	Node* n = new Node;
	n->value = value;
	n->prev = tail;
	tail->next = n;
	n->next = nullptr;
	tail = n;
}

void Sequence::additem(int pos, const ItemType& value)
{
	Node* n = new Node;
	n->value = value;
	Node* p = head;
	for (int i = 0; i < pos; i++)   //adjusting the pointer p to the position
		p = p->next;
	n->next = p;
	n->prev = p->prev;
	p->prev->next = n;
	p->prev = n;
}

int subsequence(const Sequence& seq1, const Sequence& seq2)
{
	if (seq2.size() > seq1.size() || seq2.empty())
		return -1;

	ItemType temp1, temp2;
	int i;
	for (i = 0; i < seq1.size(); i++)     //go through every node's value in seq1
	{
		seq1.get(i, temp1);
		seq2.get(0, temp2);
		if (temp1 == temp2)      //if the value is equal to seq2's first value, comparing their following nodes
		{
			int m = i, n = 0;    
			for (; m < seq1.size() && n < seq2.size(); m++, n++)
			{
				seq1.get(m, temp1);
				seq2.get(n, temp2);
				if (temp1 != temp2)   //once find difference, break the loop
					break;
			}
			if (n == seq2.size())     //if all nodes in seq2 have been examined, return the starting point i
				return i;
		}
	}
	return -1;
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
	Sequence sq1 = seq1, sq2 = seq2, rs = result;
	while (!(rs.empty()))   //delete all the nodes in result, making it empty
		rs.erase(0);
	if (sq1.empty() && sq2.empty())
	{
		result = rs;
		return;
	}

	int m = sq1.size() < sq2.size() ? sq1.size() : sq2.size();  //assign m to the smaller size
	int i = 0, n = 0;
	ItemType temp;
	for (; i < m; i++)     //interleave the node until one sequence runs out of nodes
	{
		sq1.get(i, temp);
		rs.insert(n, temp);
		n++;
		sq2.get(i, temp);
		rs.insert(n, temp);
		n++;
	}

	if (sq1.size() > sq2.size())     //add the rest of nodes
	{
		for (; i < sq1.size(); i++)
		{
			sq1.get(i, temp);
			rs.insert(n, temp);
			n++;
		}
	}
	else
	{
		for (; i < sq2.size(); i++)
		{
			sq2.get(i, temp);
			rs.insert(n, temp);
			n++;
		}
	}
	result = rs;
}
