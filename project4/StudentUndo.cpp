#include "StudentUndo.h"
#include <stack>

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch)
{
	UndoSet* new_set = new UndoSet(action, row, col, ch);          //undoset is basically a structure with action row col and character
	stack_undo.push(new_set);
}

StudentUndo::Action StudentUndo::get(int &row, int &col, int& count, std::string& text)
{
	if (stack_undo.empty())
		return Action::ERROR;
	UndoSet* curr_undo = stack_undo.top();
	row = curr_undo->m_row;
	col = curr_undo->m_col;
	text = "";
	if (curr_undo->m_action == Action::INSERT && curr_undo->m_ch == 9)       //count has to start at 4 if it is a tab insert
		count = 4;
	else
	    count = 1;
	stack_undo.pop();
	switch (curr_undo->m_action)
	{
	case Action::INSERT:
		while (!stack_undo.empty())          //batch all the insert together for undo
		{
			curr_undo = stack_undo.top();
			if (curr_undo->m_action != Action::INSERT)
				break;
			if (curr_undo->m_ch == 9)      //the tab key
				count += 4;
			else
				count++;
			stack_undo.pop();
		}
		return Action::DELETE;
		break;
	case Action::DELETE:
		text += curr_undo->m_ch;
		while (!stack_undo.empty())       //batch all the delete together for undo
		{
			int pre_col = curr_undo->m_col;
			curr_undo = stack_undo.top();
			if (curr_undo->m_action != Action::DELETE)
				break;
			if (pre_col == curr_undo->m_col)
				text.insert(0, 1, curr_undo->m_ch);
			else if (pre_col == curr_undo->m_col - 1)
				text += curr_undo->m_ch;
			else
				break;
			stack_undo.pop();
		}
		return Action::INSERT;
		break;
	case Action::SPLIT:
		return Action::JOIN;
		break;
	case Action::JOIN:
		return Action::SPLIT;
		break;
	case Action::ERROR:
		return Action::ERROR;
		break;
	}
	return Action::ERROR;
}

void StudentUndo::clear() 
{
	while (!stack_undo.empty())               //deleting all the items from stack
	{
		UndoSet* tempset = stack_undo.top();
		delete tempset;
		stack_undo.pop();
	}
}
