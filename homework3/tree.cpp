/*#include <iostream>
#include <cassert>
using namespace std;*/

int countIsIn(const double a1[], int n1, const double a2[], int n2)
{
	if (n1 <= 0)
		return 1;
	if (n2 <= 0)
		return 0;
	if (a1[0] == a2[0])
		return countIsIn(a1 + 1, n1 - 1, a2 + 1, n2 - 1) + countIsIn(a1, n1, a2 + 1, n2 - 1);
	else
		return countIsIn(a1, n1, a2 + 1, n2 - 1);
}

void exchange(double& x, double& y)
{
	double t = x;
	x = y;
	y = t;
}

void divide(double a[], int n, double divider,
	int& firstNotGreater, int& firstLess)
{
	if (n < 0)
		n = 0;

	firstNotGreater = 0;
	firstLess = n;
	int firstUnknown = 0;
	while (firstUnknown < firstLess)
	{
		if (a[firstUnknown] < divider)
		{
			firstLess--;
			exchange(a[firstUnknown], a[firstLess]);
		}
		else
		{
			if (a[firstUnknown] > divider)
			{
				exchange(a[firstNotGreater], a[firstUnknown]);
				firstNotGreater++;
			}
			firstUnknown++;
		}
	}
}

void order(double a[], int n)
{
	if (n <= 1)
		return;
	int g = 0, l = 0;
	divide(a, n, a[n / 2], g, l);
	int g1 = g;
	order(a + l, n - l);
	order(a, g1);
}

/*int main()
{
	double a[7] = { 10, 50, 40, 20, 50, 40, 30 };
	order(a, 7);
	for (int i = 0; i < 7; i++)
		cout << a[i] << endl;
	double b[5] = { 3, 4, 4, 6, 1 };
	order(b, 2);
	for (int i = 0; i < 2; i++)
		cout << b[i] << endl;

	double c[7] = { 10, 50, 40, 20, 50, 40, 30 };
	double d[3] = { 10, 20, 40 };
	double e[3] = { 10, 40, 30 };
	double f[3] = { 20, 10, 40 };
	double g[3] = { 50, 40, 30 };
	assert(countIsIn(d, 3, c, 7) == 1);
	assert(countIsIn(e, 3, c, 7) == 2);
	assert(countIsIn(f, 3, c, 7) == 0);
	assert(countIsIn(g, 3, c, 7) == 3);
	assert(countIsIn(d, 0, c, 7) == 1);
	assert(countIsIn(d, 3, c, 0) == 0);
	cout << "all tests succeeded" << endl;
}*/