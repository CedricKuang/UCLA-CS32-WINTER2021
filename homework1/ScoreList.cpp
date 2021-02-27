/*#include "ScoreList.h"
#include "Sequence.h"
using namespace std;

ScoreList::ScoreList() : m_sequence()
{
}

bool ScoreList::add(ItemType score)
{
	if (score <= 100 && score >= 0)
		return m_sequence.insert(score);
	else
		return false;
}

bool ScoreList::remove(ItemType score)
{
	int pos = m_sequence.find(score);
	return m_sequence.erase(pos);
}

int ScoreList::size() const
{
	return m_sequence.size();
}

ItemType ScoreList::minimum() const
{
	if (m_sequence.empty())
		return NO_SCORE;
	ItemType min;
	if (m_sequence.get(0, min))
		return min;
	else
		return NO_SCORE;
}

ItemType ScoreList::maximum() const
{
	if (m_sequence.empty())
		return NO_SCORE;
	ItemType max;
	if (m_sequence.get(m_sequence.size() - 1, max))
		return max;
	else
		return NO_SCORE;
}*/