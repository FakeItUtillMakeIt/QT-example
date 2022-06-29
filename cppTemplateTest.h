#pragma once

#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <vector>

using namespace std;

template <class T>
class MyClass
{
public:
	MyClass() {

	}
	~MyClass() {

	}


	void push(T const&);
	void pop();
	T top() const;


private:
	vector<T> elems;
};

template<class T>
void MyClass<T>::push(T const&) {

}

template<class T>
void MyClass<T>::pop() {

}

template<class T>
T MyClass<T>::top() const{

}

#endif
