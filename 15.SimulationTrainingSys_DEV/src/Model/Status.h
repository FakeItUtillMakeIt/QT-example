#pragma once

#ifndef MYSTATUS_H
#define MYSTATUS_H

#include <string>
#include <map> 

using namespace std;
class Status
{
public:
	Status();
	~Status();

	int m_id;
	string m_name; 
};

#endif