#pragma once
#include <vector>
#include <string>
#include <fstream>

class ListWords
{
	std::vector<std::string> m_words;
	int m_indexWord = 0;

	void LoadFile() {
		std::ifstream wordsFile("words.txt");
		std::string line;
		if (wordsFile.is_open()) {
			while (std::getline(wordsFile, line)) m_words.push_back(line);
			wordsFile.close();
		}
	}

public:
	ListWords() { LoadFile(); };
	~ListWords() = default;

	std::string Current() { return m_words[m_indexWord]; };
	void Next() { ++m_indexWord; };
	int CurrentIndex() const { return m_indexWord; };

	int Size() const { return m_words.size();};
	
};
