/*#include <iostream>
#include <cassert>
using namespace std;

bool somePredicate(double x)
{
	return x < 0;
}*/

bool anyTrue(const double a[], int n)
{
	if (n < 0)
		return false;
	if (n == 0)
		return false;
	else if (somePredicate(a[n - 1]))
		return true;
	else
		return anyTrue(a, n - 1);
}

int countTrue(const double a[], int n)
{
	if (n < 0)
		return 0;
	if (n == 0)
		return 0;
	else if (somePredicate(a[n - 1]))
		return 1 + countTrue(a, n - 1);
	else
		return 0 + countTrue(a, n - 1);
}

int firstTrue(const double a[], int n)
{
	if (n < 0)
		return -1;
	if (n == 0)
		return -1;
	else if (somePredicate(a[n - 1]) && firstTrue(a, n - 1) == -1)
		return n - 1;
	else
		return firstTrue(a, n - 1);
}

int positionOfMax(const double a[], int n)
{
	if (n <= 0)
		return -1;
	if (n == 1)
		return 0;
	if (n == 2)
	{
		if (a[0] >= a[1])
			return 0;
		else
			return 1;
	}
	if (a[n - 1] > a[positionOfMax(a, n - 1)])
		return n - 1;
	else
		return positionOfMax(a, n - 1);
}

bool isIn(const double a1[], int n1, const double a2[], int n2)
{
	if (n2 <= 0)
		return false;
	if (n1 <= 0)
		return true;
	if (a1[0] == a2[0])
		return isIn(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
	else
		return isIn(a1, n1, a2 + 1, n2 - 1);
}

/*int main()
{
	double a[5] = { 1, 2, -3, -4, 5 };
	assert(anyTrue(a, 5));
	assert(!anyTrue(a, 2));
	assert(!anyTrue(a, -1));
	assert(countTrue(a, 5) == 2);
	assert(countTrue(a, 2) == 0);
	assert(countTrue(a, -1) == 0);
	assert(countTrue(a, 0) == 0);
	assert(firstTrue(a, 5) == 2);
	assert(firstTrue(a, 2) == -1);
	assert(firstTrue(a, 0) == -1);
	assert(firstTrue(a, -1) == -1);
	assert(positionOfMax(a, 5) == 4);
	assert(positionOfMax(a, 1) == 0);
	assert(positionOfMax(a, 0) == -1);
	double b[5] = { 2, 2, 2, 2, 2 };
	assert(positionOfMax(b, 5) == 0);
	double c[3] = { 2, -3, -4 };
	assert(isIn(c, 3, a, 5));
	assert(!isIn(c, 3, a, 2));
	assert(isIn(c, 0, a, 5));
	assert(isIn(c, -1, a, 5));
	assert(!isIn(c, 3, a, -1));
	double d[5] = { 1, -2, -3, -4, 5 };
	assert(firstTrue(d, 5) == 1);
	cout << "all tests succeeded" << endl;
}*/