#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>

class StudentSpellCheck : public SpellCheck {
public:
	StudentSpellCheck();
	virtual ~StudentSpellCheck();
	bool load(std::string dict_file);
	bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
	void spellCheckLine(const std::string& line, std::vector<Position>& problems);

private:
	std::string alphabet;
	struct Node
	{
		char ch;
		int depth;
		Node* children_node[28];
		Node()
		{
			for (int i = 0; i < 28; i++)
				children_node[i] = nullptr;
		}
	};
	Node* root;
	void free_space(Node* node);
	bool insert_word(std::string word);
	int char_to_int(char ch);
	void add_falg_node(Node* curr, int i);
};

#endif  // STUDENTSPELLCHECK_H_
