#pragma once
#include <vector>

namespace SerraPlo {

class IApp;
class IScreen;

class ScreenList
{
protected:
	std::vector<IScreen*> m_screens;
	IApp* m_game = nullptr;
	int m_currentScreenIndex;
public:
	explicit ScreenList(IApp* game);
	~ScreenList();

	IScreen* moveNext();
	IScreen* movePrev();

	void setScreen(int nextScreen);
	void addScreen(IScreen* newScreen);

	void destroy();

	IScreen* getCurScreen() const;;
};

}
