#pragma once

#ifndef ROCKETTYPE_H
#define ROCKETTYPE_H

#include <string>
#include <map> 

using namespace std;
class RocketType
{
public:
	RocketType();
	~RocketType();

	int m_id;
	string m_name;
	string m_code;
};

#endif