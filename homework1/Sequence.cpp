#include "Sequence.h"
using namespace std;

Sequence::Sequence()
{
	m_nsq = 0;
}

bool Sequence::empty() const
{
	return m_nsq == 0;
}

int Sequence::size() const
{
	return m_nsq;
}

int Sequence::insert(int pos, const ItemType& value)
{
	if (pos < 0 || pos > m_nsq || m_nsq == DEFAULT_MAX_ITEMS)
		return - 1;

	if (pos == m_nsq)
	{
		m_sq[pos] = value;
		m_nsq++;
		return pos;
	}

	for (int i = m_nsq; i > pos; i--)
		m_sq[i] = m_sq[i - 1];
	m_sq[pos] = value;
	m_nsq++;
	return pos;
}

int Sequence::insert(const ItemType& value)
{
	if (m_nsq == DEFAULT_MAX_ITEMS)
		return -1;

	for (int p = 0; p <= m_nsq; p++)
	{
		if (m_nsq == p || value < m_sq[p])
		{
			if (insert(p, value) != -1)
				return p;
			else
				return -1;
		}
	}
	return -1;
}

bool Sequence::erase(int pos)
{
	if (pos < 0 || pos >= m_nsq)
		return false;

	for (int i = pos; i < m_nsq; i++)
	{
		m_sq[i] = m_sq[i + 1];
	}
	m_nsq--;
	return true;
}

int Sequence::remove(const ItemType& value)
{
	int count = 0;
	for (int i = 0; i < m_nsq; i++)
	{
		if (m_sq[i] == value)
		{
			if (erase(i))
			{
				count++;
				i--;
			}
		}
	}
	return count;
}

bool Sequence::get(int pos, ItemType& value) const
{
	if (pos < 0 || pos >= m_nsq)
		return false;

	value = m_sq[pos];
	return true;
}

bool Sequence::set(int pos, const ItemType& value)
{
	if (pos < 0 || pos >= m_nsq)
		return false;

	m_sq[pos] = value;
	return true;
}

int Sequence::find(const ItemType& value) const
{
	int p;
	for (p = 0; p < m_nsq; p++)
		if (m_sq[p] == value)
			return p;
	p = -1;
	return p;
}

void Sequence::swap(Sequence& other)
{
	Sequence temp;
	temp = *this;
	*this = other;
	other = temp;
}

ItemType Sequence::visit(int pos) const
{
	return m_sq[pos];
}

void Sequence::override(int pos, const ItemType& value)
{
	m_sq[pos] = value;
}

void Sequence::setsize(int size)
{
	m_nsq = size;
}