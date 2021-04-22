#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cctype>

SpellCheck* createSpellCheck()
{
	return new StudentSpellCheck;
}

StudentSpellCheck::StudentSpellCheck()
{
	alphabet = "abcdefghijklmnopqrstuvwxyz.";
	Node* new_node1 = new Node;
	root = new_node1;
	root->ch = ' ';
	root->depth = 0;
}

StudentSpellCheck::~StudentSpellCheck()
{
	free_space(root);
}

bool StudentSpellCheck::load(std::string dictionaryFile)
{
	std::ifstream infile(dictionaryFile);
	if (!infile)					//return false if open unsuccessfully
		return false;
	std::string temp;
	while (getline(infile, temp))
		if (!insert_word(temp))     //insert each word into trie structure
			return false;
	return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions)
{
	Node* curr = root;
	int i;
	for (i = 0; i < word.size(); i++)
	{
		word[i] = tolower(word[i]);
		if (curr->children_node[char_to_int(word[i])] != nullptr)
			curr = curr->children_node[char_to_int(word[i])];
		else
			break;
	}
	if (i == word.size() && curr->children_node[27] != nullptr && curr->children_node[27]->ch == '$')		//if all the characters exist in the nodes, there should be a mark point if the word is valid
		return true;
	suggestions.clear();
	for (int m = 0; m < word.size(); m++)
	{
		for (int n = 0; n < 26; n++)
		{
			curr = root;
			std::string temp_word = word;
			temp_word[m] = 97 + n;				//change each character in the word to other different characters (up to 26), then check the new word valid or not
			int q;
			for (q = 0; q < word.size(); q++)
			{
				if (curr->children_node[char_to_int(temp_word[q])] != nullptr)
					curr = curr->children_node[char_to_int(temp_word[q])];
				else
					break;
			}
			if (suggestions.size() != max_suggestions && q == word.size() && curr->children_node[27] != nullptr && curr->children_node[27]->ch == '$')
				suggestions.push_back(temp_word);		//if the new word is valid, insert it into suggestions
		}
		if (suggestions.size() >= max_suggestions)
			break;
	}
	return false;
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems)
{
	problems.clear();
	for (int i = 0; i < line.size(); i++)   //get each word's position from the line 
	{
		if (!isalpha(line[i]) && line[i] != '\'')
			continue;
		SpellCheck::Position pos;
		pos.start = i;
		while (i < line.size() && (isalpha(line[i]) || line[i] == '\''))
			i++;
		i--;
		pos.end = i;
		problems.push_back(pos);
	}
	std::vector<SpellCheck::Position>::iterator it = problems.begin();
	for (; it != problems.end();)
	{
		Node* curr = root;
		std::string temp_string = line.substr((*it).start, (*it).end - (*it).start + 1);   //get the specific substring from the line
		int i;
		for (i = 0; i < temp_string.size(); i++)
		{
			if (curr->children_node[char_to_int(tolower(temp_string[i]))] != nullptr)
				curr = curr->children_node[char_to_int(tolower(temp_string[i]))];
			else
				break;
		}
		if (temp_string.size() == i && curr->children_node[27] != nullptr && curr->children_node[27]->ch == '$')    //if the word exist in the dictionary, delete the position from problems
			it = problems.erase(it);
		else
			it++;
	}
}

void StudentSpellCheck::free_space(Node* node)
{
	if (node == nullptr)         //recursively delete all the nodes from the trie in postorder
		return;
	for (int i = 0; i < 27; i++)
		free_space(node->children_node[i]);
	delete node;
}

bool StudentSpellCheck::insert_word(std::string word)
{
	Node* curr = root;
	int i = 0;
	for (; i < word.size(); i++)
	{
		if (!isalpha(word[i]) && word[i] != '\'')
			return false;
		if (curr->children_node[char_to_int(word[i])] != nullptr)      //if the corresponding node exist, go to the next node
			curr = curr->children_node[char_to_int(word[i])];
		else														   //if not add new node
		{
			Node* new_node = new Node;
			new_node->ch = word[i];
			new_node->depth = i + 1;
			curr->children_node[char_to_int(word[i])] = new_node;
			curr = curr->children_node[char_to_int(word[i])];
		}
	}
	add_falg_node(curr, i);     //flag node marks the end of the word
	return true;
}

int StudentSpellCheck::char_to_int(char ch)
{
	if (ch == 39)
		return 26;      //the 26 spot of children_node is especially reserved for apostrophe
	else
		return ch - 97;
}

void StudentSpellCheck::add_falg_node(Node* curr, int i)
{
	Node* new_node = new Node;            //add the 27 node (mark point) to the trie
	new_node->ch = '$';
	new_node->depth = i + 1;
	curr->children_node[27] = new_node;
}