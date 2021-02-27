#include "Sequence.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	Sequence a;
	assert(a.empty() == 1);
	a.insert(10);
	a.insert(30);
	a.insert(20);
	a.insert(0, 0);
	a.insert(4, 50);
	a.insert(4, 40);
	ItemType x = 0;
	assert(a.get(0, x) && x == 0);
	assert(a.get(1, x) && x == 10);
	assert(a.get(2, x) && x == 20);
	assert(a.get(3, x) && x == 30);
	assert(a.get(4, x) && x == 40);
	assert(a.get(5, x) && x == 50);
	Sequence b = a;
	assert(b.get(4, x) && x == 40);
	Sequence c;
	c = a;
	assert(c.get(4, x) && x == 40);
	assert(a.erase(0) && a.get(0, x) && x == 10);
	assert(a.erase(4) && a.get(3, x) && x == 40);
	assert(a.erase(2) && a.get(1, x) && x == 20);
	Sequence d;
	for (int i = 0; i < 4; i++)
		d.insert(10);
	d.insert(20);
	int n = d.remove(10);
	assert(n == 4 && d.get(0, x) && x == 20);
	n = d.remove(3);
	assert(n == 0);
	assert(a.set(0, 100) && a.get(0, x) && x == 100);
	assert(a.set(1, 200) && a.get(1, x) && x == 200);
	assert(a.set(2, 400) && a.get(2, x) && x == 400);
	assert(!(a.set(3, 300)));
	int f = a.find(100);
	assert(f == 0);
	f = a.find(1000);
	assert(f == -1);
	Sequence e;
	e.insert(10);
	e.insert(20);
	e.insert(40);
	e.swap(a);
	assert(e.get(2, x) && x == 400);
	assert(a.get(2, x) && x == 40);
	Sequence sq1, sq2, sq3;
	for (int i = 1; i < 6; i++)
		sq1.insert(i);
	for (int i = 3; i < 5; i++)
		sq2.insert(i);
	assert(subsequence(sq1, sq2) == 2);

	interleave(sq1, sq2, sq1);
	assert(sq1.get(0, x) && x == 1);
	assert(sq1.get(1, x) && x == 3);
	assert(sq1.get(2, x) && x == 2);
	assert(sq1.get(3, x) && x == 4);
	cout << "Passed all tests" << endl;
}

/*#include "Sequence.h"
#include <iostream>
#include <cassert>

using namespace std;

void test()
{
	cerr << "test A" << endl;
	Sequence s;
	cerr << "test B" << endl;
	s.insert(0, IntWrapper(10));
	cerr << "test C" << endl;
	s.insert(0, IntWrapper(20));
	cerr << "test D" << endl;
	Sequence s2;
	cerr << "test E" << endl;
	s2.insert(0, IntWrapper(30));
	cerr << "test F" << endl;
	s2 = s;
	cerr << "test G" << endl;
	s2.insert(0, IntWrapper(40));
	cerr << "test H" << endl;
}

int main()
{
	test();
	cerr << "DONE" << endl;
}*/
