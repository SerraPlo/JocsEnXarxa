#include "ScreenList.h"
#include "IGameScreen.h"

namespace SerraPlo {

ScreenList::ScreenList(IMainGame* game) :
	m_game(game),
	m_currentScreenIndex(SCREEN_INDEX_NO_SCREEN){}

ScreenList::~ScreenList() {
	destroy();
}

IGameScreen* ScreenList::moveNext() {
	auto currentScreen = getCurScreen();
	if(currentScreen->getNextScreenIndex() != SCREEN_INDEX_NO_SCREEN) {
		m_currentScreenIndex = currentScreen->getNextScreenIndex();
	}
	return getCurScreen();
}

IGameScreen* ScreenList::movePrev() {
	auto currentScreen = getCurScreen();
	if (currentScreen->getPrevScreenIndex() != SCREEN_INDEX_NO_SCREEN) {
		m_currentScreenIndex = currentScreen->getPrevScreenIndex();
	}
	return getCurScreen();
}

void ScreenList::setScreen(int nextScreen) {
	m_currentScreenIndex = nextScreen;
}

void ScreenList::addScreen(IGameScreen* newScreen) {
	newScreen->m_screenIndex = m_screens.size();
	m_screens.push_back(newScreen);
	newScreen->build();
	newScreen->setParentGame(m_game);
}

void ScreenList::destroy() {
	for (auto s : m_screens) s->destroy();
	m_screens.resize(0);
	m_currentScreenIndex = SCREEN_INDEX_NO_SCREEN;
}

IGameScreen* ScreenList::getCurScreen() const {
	if (m_currentScreenIndex == SCREEN_INDEX_NO_SCREEN) return nullptr;
	return m_screens[m_currentScreenIndex];
}
}
