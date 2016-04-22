#pragma once
#include <vector>

namespace SerraPlo {

class IMainGame;
class IGameScreen;

class ScreenList
{
protected:
	std::vector<IGameScreen*> m_screens;
	IMainGame* m_game = nullptr;
	int m_currentScreenIndex;
public:
	explicit ScreenList(IMainGame* game);
	~ScreenList();

	IGameScreen* moveNext();
	IGameScreen* movePrev();

	void setScreen(int nextScreen);
	void addScreen(IGameScreen* newScreen);

	void destroy();

	IGameScreen* getCurScreen() const;;
};

}
