#include "common.h"

#include <math.h>


std::string IntToStr(int n)
{
	std::string s = "";

	while (n)
	{
		s += '0' + (n % 10);
		n /= 10;
	};

	//reverse
	char c;
	for (int i = 0; i < s.length()/2; i++)
	{
		c = s[i];
		s[i] = s[s.length()-i-1];
		s[s.length()-i-1] = c;
	}
	
	if (!s.length())
		s = "0";


	return s;
}

double Abs(double sk)
{
	if (sk < 0)
        return -sk;
	return sk;
}


template <class T>
void Swap(T &a, T &b)
{
	T temp = a;
	a = b;
	b = temp;
}
