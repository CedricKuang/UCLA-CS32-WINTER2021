#include <iostream>
#include <string>
#include <stack>
#include <cassert>
#include <cctype>
using namespace std;

bool checkprecedence(const char p1, const char p2);
bool checknumber(const string& infix, int i);
bool checkparenthese1(const string& infix, int i);
bool checkparenthese2(const string& infix, int i);
bool checkoperator(const string& infix, int i);

int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
    if (infix == "")
        return 1;
    postfix = "";
    stack<char> s_operator;
    for (int i = 0; i < infix.size(); i++)
    {
        switch (infix[i])
        {
        case '0': case '1': case '2': case '3':
        case '4': case '5': case '6': case '7':
        case '8': case '9':
            if (!(checknumber(infix, i)))
                return 1;
            postfix += infix[i];
            break;
        case '(':
            if (!(checkparenthese1(infix, i)))
                return 1;
            s_operator.push(infix[i]);
            break;
        case ')':
            if (!(checkparenthese2(infix, i)))
                return 1;
            while (!(s_operator.empty()) && s_operator.top() != '(')
            {
                postfix += s_operator.top();
                s_operator.pop();
            }
            if (s_operator.empty())    //it shouldn't be empty at here if evrey ) is already paired with a (
                return 1;
            s_operator.pop();
            break;
        case '|': case '&': case '!':
            if (!(checkoperator(infix, i)))
                return 1;
            while (!(s_operator.empty()) && s_operator.top() != '(' && checkprecedence(infix[i], s_operator.top()))
            {
                postfix += s_operator.top();
                s_operator.pop();
            }
            s_operator.push(infix[i]);
            break;
        case ' ':
            break;
        default:
            return 1;
        }
    }

    if (s_operator.empty() && postfix == "")
        return false;
    while (!(s_operator.empty()))
    {
        if (s_operator.top() == '(' || s_operator.top() == ')')   //not enough parentheses are paired, still left in the stack
            return 1;
        postfix += s_operator.top();
        s_operator.pop();
    }

    stack<char> s_operand;
    for (int i = 0; i < postfix.size(); i++)
    {
        if (isdigit(postfix[i]))
        {
            s_operand.push(postfix[i]);
            continue;
        }
        bool r;
        if (postfix[i] == '!')
        {
            int operand2 = s_operand.top() - 48;
            s_operand.pop();
            r = (!(values[operand2]));
        }
        else
        {
            int operand2 = s_operand.top() - 48;
            s_operand.pop();
            int operand1 = s_operand.top() - 48;
            s_operand.pop();
            if (postfix[i] == '&')
                r = (values[operand2] && values[operand1]);
            if (postfix[i] == '|')
                r = (values[operand2] || values[operand1]);
        }
        char num;
        for (int n = 0; n < 10; n++)
            if (values[n] == r)
            {
                num = n + 48;
                break;
            }
        s_operand.push(num);  
    }
    int res = s_operand.top() - 48;
    result = values[res];
    return 0;
}

bool checkprecedence(const char p1, const char p2)
{
    string precedence = "|&!";      //check precedence by using the order of characters in a string
    int p1n = precedence.find(p1);
    int p2n = precedence.find(p2);
    if (p1n <= p2n)
        return true;
    else
        return false;
}

bool checknumber(const string& infix, int i)
{
    if (i == 0)             //if a number is at first of infix, return true
        return true;

    int n = i - 1;
    while (n >= 0 && infix[n] == ' ')     //find the first non-empty characters before i 
        n--;

    if (n < 0)          //it is ok that all charcters before a digit are empty spaces
        return true;
    if (isdigit(infix[n]))     //There shouldn't be a digit before a digit
        return false;
    if (infix[n] == ')')       //there shouldn't be a ) before a digit
        return false;
    return true;
}

bool checkparenthese1(const string& infix, int i)
{
    if (i == 0)             //if a parenthesis is at first of infix, return true
        return true;

    int n = i - 1;
    while (n >= 0 && infix[n] == ' ')     //find the first non-empty characters before i 
        n--;

    if (n < 0)          //it is ok that all charcters before a parenthesis are empty spaces
        return true;
    if (isdigit(infix[n]))     //There shouldn't be a digit before a parenthesis
        return false;
    if (infix[n] == ')')       //there shouldn't be a paired parenthesis right before a parenthesis
        return false;
    return true;
}

bool checkparenthese2(const string& infix, int i)
{
    if (i == 0)         //the second parenthesis can't be on the front itself
        return false;

    int n = i - 1;
    while (n >= 0 && infix[n] == ' ')     //find the first non-empty characters before i 
        n--;

    if (n < 0)         //the second parenthesis shouldn't be at first
        return false;
    if (isdigit(infix[n]))    //it's ok to have a digit before it
        return true;
    if (infix[n] == ')')      //it's ok to have a ) before it
        return true;
    return false;
}

bool checkoperator(const string& infix, int i)
{
    if (infix[i] == '!')
    {
        if (i == 0)             //it's ok for a ! to be the first
            return true;

        int m = i + 1;
        while (m < infix.size() && infix[m] == ' ')    //find the first non-empty character after !
            m++;
        if (m == infix.size())      //! cannot be the last character
            return false;

        int n = i - 1;
        while (n >= 0 && infix[n] == ' ')     //find the first non-empty characters before i 
            n--;

        if (n < 0)          //it is ok that all charcters before a ! are empty spaces
            return true;
        if (isdigit(infix[n]))
            return false;    //there souldn't be a digit before !
        if (infix[n] == ')')
            return false;      //there shouldn't be a ) before !

        return true;
    }
    else       //& and |
    {
        if (i == 0)         //& and | can't be on the front itself
            return false;

        int m = i + 1;
        while (m < infix.size() && infix[m] == ' ')    //find the first non-empty character after & or |
            m++;
        if (m == infix.size())      //& and | cannot be the last character
            return false;

        int n = i - 1;
        while (n >= 0 && infix[n] == ' ')     //find the first non-empty characters before i 
            n--;

        if (n < 0)         //& and | shouldn't be at first
            return false;
        if (isdigit(infix[n]))    //digit can be before & and |
            return true;
        if (infix[n] == ')')      //) can be before & and |
            return true;

        return false;
    }
}

int main()
{
    
    bool ba[10] = {
        //  0      1      2      3      4      5      6      7      8      9
          true,  true,  true,  false, false, false, true,  false, true,  false
    };
    string pf;
    bool answer;
    assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" && answer);
    assert(evaluate("8|", ba, pf, answer) == 1);
    assert(evaluate(" &6", ba, pf, answer) == 1);
    assert(evaluate("4 5", ba, pf, answer) == 1);
    assert(evaluate("01", ba, pf, answer) == 1);
    assert(evaluate("()", ba, pf, answer) == 1);
    assert(evaluate("()4", ba, pf, answer) == 1);
    assert(evaluate("2(9|8)", ba, pf, answer) == 1);
    assert(evaluate("2(&8)", ba, pf, answer) == 1);
    assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
    assert(evaluate("x+5", ba, pf, answer) == 1);
    assert(evaluate("", ba, pf, answer) == 1);
    assert(evaluate("2|3|4", ba, pf, answer) == 0
        && pf == "23|4|" && answer);
    assert(evaluate("2|(3|4)", ba, pf, answer) == 0
        && pf == "234||" && answer);
    assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
        && pf == "43!03&&|" && !answer);
    assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9" && !answer);
    ba[2] = false;
    ba[9] = true;
    assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9" && answer);
    assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" && !answer);
    cout << "all tests succeeded" << endl;
}