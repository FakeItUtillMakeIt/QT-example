#pragma once

#ifndef SINGLE_DATA_CONTROLLER_H
#define SINGLE_DATA_CONTROLLER_H


#include "MysqlOP.h"
#include <vector>
#include <map>
#include <string>

using namespace std;


class SingleDataController
{
private:
	SingleDataController();

	static SingleDataController* instance;

public:
	static SingleDataController* getInstance() {
		if (instance == nullptr) {
			instance = new SingleDataController();
		}
		return instance;
	}


	map<int, vector<string>> getDataBaseParamInfo();

};

#endif