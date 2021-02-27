#include <iostream>
#include <cassert>
#include <string>
#include "Sequence.h"
using namespace std;

void test()
{
    Sequence s;
    s.insert(0, 10);
    s.insert(0, 20);
    assert(s.size() == 2);
    ItemType x = 999;
    assert(s.get(0, x) && x == 20);
    assert(s.get(1, x) && x == 10);
}

int main()
{
    Sequence s;
    assert(s.empty());
    assert(s.find(42) == -1);
    assert(s.insert(42) == 0);
    assert(s.size() == 1 && s.find(42) == 0);
    Sequence a, b;
    a.insert(3);
    a.insert(4);
    b.insert(5);
    b.insert(6);
    a.swap(b);
    assert(a.find(5) == 0 && a.find(6) == 1);
    cout << "Passed all tests" << endl;
    test();
    cout << "Passed all tests" << endl;
}


