#pragma once
#include <vector>
#include <string>

class ListWords
{
	std::vector<std::string> m_words;
	int m_indexWord = 0;
public:
	ListWords() {
		LoadFile();
	};
	~ListWords() = default;
	std::string Current() { return m_words[m_indexWord]; };
	void Next() {m_indexWord++;};
	void LoadFile() {
		m_words.resize(5);
		for (int i = 0; i < 5; i++) m_words[i] = "paraula" + std::to_string(i);
	}
};
