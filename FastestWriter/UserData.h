#pragma once
#include <string>
#include <mutex>

class UserData
{
	std::string m_word;
	std::mutex m_blocker;
	bool m_sentToServer=true;
public:
	void SetWord(const std::string &str) {
		m_blocker.lock();
		m_word = str;
		m_blocker.unlock();
		m_sentToServer = false;
	}
	bool GetWord(std::string &str) {
		str = m_word;
		if (m_sentToServer) return false;
		else {
			m_sentToServer = true;
			return true;
		}
	}
	UserData() = default;
	~UserData() = default;
};

