#pragma once
#include "UserData.h"
#include <iostream>
class UserInput
{
	UserData* m_data;
public:

	UserInput(UserData * a) {
		m_data = a;
	}
	~UserInput() = default;
	void operator()() {
		std::string temp;
		while (true) {
			std::cin >> temp;
			m_data->SetWord(temp);
		}
	}
};

