#include<iostream>
#include<string>
using namespace std;
class Complex{
	friend Complex operator+(Complex c1, Complex c2);
	friend Complex operator++(Complex&c1,int);
private:
	int a;
	int b;
public:
	friend Complex& operator++(Complex&c1);
	Complex &operator--();
	Complex operator-(Complex c2);
	Complex(int a = 0, int b = 0)
	{
		this->a = a;
		this->b = b;
	}
	~Complex(){}
};
Complex operator+(Complex c1, Complex c2)
{
	Complex temp;
	temp.a = c1.a + c2.a;
	temp.b = c1.b + c2.b;
	return temp;
}
Complex Complex::operator-(Complex c2)
{
	this->a -= c2.a;
	this->b -= c2.b;
	return *this;
	//return Complex(this->a - c2.a, this->b - c2.b);
}
Complex& Complex::operator--()
{
	this->a--;
	this->b--;
	return *this;
}
//重载前置++
Complex& operator++(Complex&c1)
{
	c1.a++;
	c1.b++; 
	return c1;

}
//重载后置++
Complex operator++(Complex&c1,int)
{
	Complex temp = c1;
	c1.a++;
	c1.b++;
	return temp;

}
void main()
{
	Complex c1(1, 2);
	Complex c2(2, 3);
	Complex c3 = c1 + c2;
	c1 - c2;
	++c1;
	c1--;
	int a = 0;

}