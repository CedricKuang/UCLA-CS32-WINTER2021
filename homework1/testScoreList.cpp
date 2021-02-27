/*#include <iostream>
#include <cassert>
#include "Sequence.h"
#include "ScoreList.h"
using namespace std;


int main()
{
    ScoreList sl1;
    sl1.add(70);
    sl1.add(80);
    sl1.add(90);
    assert(sl1.add(100));
    assert(!(sl1.add(-100)));
    assert(sl1.remove(70));
    assert(!(sl1.remove(77)));
    sl1.add(70);
    assert(sl1.size() == 4);
    assert(sl1.minimum() == 70);
    assert(sl1.maximum() == 100);
    cout << "Passed all tests" << endl;
}*/