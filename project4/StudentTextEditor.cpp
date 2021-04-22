#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>

TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo) {
	m_col = m_row = m_lines = 0;
}

StudentTextEditor::~StudentTextEditor()
{
	reset();
}

bool StudentTextEditor::load(std::string file) 
{
	if (!text_lines.empty())
		reset();

	std::ifstream infile(file);
	if (!infile)					//return false if open unsuccessfully
		return false;

	std::string* temp = new std::string;		//dynamically allocate memory and insert each line into list
	while (getline(infile, *temp))
	{
		/*int m = temp->find('\n');
		int n = temp->find('\r');
		if (m != temp->npos)          
			temp->erase(m);
		if (n != temp->npos)
			temp->erase(n);*/
		if (*temp != "" && (*temp)[temp->size() - 1] == '\r')    //remove the \r from the line (if there is) 
			temp->erase(temp->size() - 1);
		text_lines.push_back(temp);
		temp = new std::string;
	}
	m_col = m_row = 0;
	m_it = text_lines.begin();
	m_lines = text_lines.size();
	return true;
}

bool StudentTextEditor::save(std::string file) 
{
	std::ofstream outfile(file);
	if (!outfile)					//return false if create file unsuccessfully
		return false;

	std::list<std::string*>::iterator it = text_lines.begin();			//iterate through the list, output each line into the file
	for (; it != text_lines.end(); it++)
		outfile << (**it) << "\n";
	return true;
}

void StudentTextEditor::reset()
{
	std::list<std::string*>::iterator it = text_lines.begin();
	while (it != text_lines.end())			//free all the memory
	{
		delete* it;
		it = text_lines.erase(it);
	}
	m_row = m_col = 0;
	getUndo()->clear();
}

void StudentTextEditor::move(Dir dir)
{
	switch (dir)
	{
	case Dir::UP:
		if (m_row != 0)
		{
			m_row--;
			m_it--;
			if (m_col > (*m_it)->size())
				m_col = (*m_it)->size();
		}
		break;
	case Dir::DOWN:
		if (m_row != m_lines - 1)
		{
			m_row++;
			m_it++;
			if (m_col > (*m_it)->size())
				m_col = (*m_it)->size();
		}
		break;
	case Dir::LEFT:
		if (m_col != 0)
			m_col--;
		else if (m_row != 0)
		{
			m_row--;
			m_it--;
			m_col = (*m_it)->size();		//(*m_it)->size() is the position right after the last character in the current line, 
		}		
		break;                              //which is also the number of total characters in this line
	case Dir::RIGHT:
		if (m_col != (*m_it)->size())
			m_col++;
		else if (m_row != m_lines - 1)
		{
			m_row++;
			m_it++;
			m_col = 0;
		}
		break;
	case Dir::HOME:
		m_col = 0;
		break;
	case Dir::END:
		m_col = (*m_it)->size();
		break;
	}
}

void StudentTextEditor::del() 
{
	if (m_col != (*m_it)->size())
	{
		char del_ch = (**m_it)[m_col];
		(*m_it)->erase(m_col, 1);
		getUndo()->submit(Undo::Action::DELETE, m_row, m_col, del_ch);
	}
	else if (m_row != m_lines && m_col == (*m_it)->size())      //if at the end of the current line
	{
		std::list<std::string*>::iterator temp = m_it;
		(*m_it)->append(**(++temp));		//append next line to current line 
		delete* temp;                   	//delete the next line
		text_lines.erase(temp);
		m_lines--;
		getUndo()->submit(Undo::Action::JOIN, m_row, m_col);
	}
}

void StudentTextEditor::backspace()
{
	if (m_col > 0)         //in essense, backspace is to delete the character before th cursor
	{
		m_col--;
		del();
	}
	else if (m_row > 0 && (m_col == 0 || (*m_it)->empty()))
	{
		m_row--;
		m_it--;
		m_col = (*m_it)->size();
		del();
	}
}

void StudentTextEditor::insert(char ch)
{
	if (ch == 9)         //9 is the ASCII code for tab
	{
		(*m_it)->insert(m_col, 4, ' ');
		m_col += 4;
	}
	else
	{
		(*m_it)->insert(m_col, 1, ch);
		m_col++;
	}
	getUndo()->submit(Undo::Action::INSERT, m_row, m_col, ch);
}

void StudentTextEditor::enter() 
{
	getUndo()->submit(Undo::Action::SPLIT, m_row, m_col);
	std::string* new_str = new std::string;
	*new_str = (*m_it)->substr(m_col);        //get the substring from the current line
	(*m_it)->erase(m_col);
	m_it++;
	text_lines.insert(m_it, new_str);		  //insert the line into correct position
	m_it--;

	m_row++;
	m_col = 0;
	m_lines++;
}

void StudentTextEditor::getPos(int& row, int& col) const
{
	row = m_row;
	col = m_col;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const 
{
	if (startRow < 0 || numRows < 0 || startRow > m_lines)
		return -1;

	lines.clear();
	if (startRow >= m_lines)
		return 0;
	std::list<std::string*>::const_iterator it = text_lines.begin();
	std::advance(it, startRow);
	for (int i = startRow; it != text_lines.end() && i < startRow + numRows; it++, i++)           //insert numRows lines into lines starting from startRow
		lines.push_back(**it);
	return lines.size();
}

void StudentTextEditor::undo() 
{
	int row = 0;
	int col = 0;
	int count = 0;
	Undo::Action act;
	std::string text = "";
	act = getUndo()->get(row, col, count, text);
	if (act != Undo::Action::ERROR)
	{
		m_row = row;
		m_col = col;
		m_it = text_lines.begin();
		advance(m_it, m_row);
	}
	switch (act)
	{
	case Undo::Action::INSERT:
		for (int i = 0; i < text.size(); i++)
		{
			if (text[i] == 9)
			{
				(*m_it)->insert(m_col, 4, ' ');
				m_col += 4;
			}
			else
			{
				(*m_it)->insert(m_col, 1, text[i]);
				m_col++;
			}
		}
		m_row = row;
		m_col = col;
		break;
	case Undo::Action::DELETE:
		while (count > 0)                         //count stands for number of characteres need to be deleted
		{
			m_col--;
			(*m_it)->erase(m_col, 1);
			count--;
		}
		break;
	case Undo::Action::SPLIT:
	{
		std::string* new_str = new std::string;
		*new_str = (*m_it)->substr(m_col);        //get the substring from the current line
		(*m_it)->erase(m_col);
		m_it++;
		text_lines.insert(m_it, new_str);		  //insert the line into correct position
		m_it--;
		m_row = row;
		m_col = col;
		m_it--;
		break;
	}
	case Undo::Action::JOIN:
	{
		std::list<std::string*>::iterator temp = m_it;
		(*m_it)->append(**(++temp));		//append next line to current line 
		delete* temp;                   	//delete the next line
		text_lines.erase(temp);
		break;
	}
	case Undo::Action::ERROR:
		break;
	}
}
