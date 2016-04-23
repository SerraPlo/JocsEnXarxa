#include "ScreenList.h"
#include "IScreen.h"

namespace SerraPlo {

ScreenList::ScreenList(IApp* game) :
	m_game(game),
	m_currentScreenIndex(SCREEN_INDEX_NO_SCREEN){}

ScreenList::~ScreenList() {
	destroy();
}

IScreen* ScreenList::moveNext() {
	auto currentScreen = getCurScreen();
	if(currentScreen->getNextScreenIndex() != SCREEN_INDEX_NO_SCREEN) {
		m_currentScreenIndex = currentScreen->getNextScreenIndex();
	}
	return getCurScreen();
}

IScreen* ScreenList::movePrev() {
	auto currentScreen = getCurScreen();
	if (currentScreen->getPrevScreenIndex() != SCREEN_INDEX_NO_SCREEN) {
		m_currentScreenIndex = currentScreen->getPrevScreenIndex();
	}
	return getCurScreen();
}

void ScreenList::setScreen(int nextScreen) {
	m_currentScreenIndex = nextScreen;
}

void ScreenList::addScreen(IScreen* newScreen) {
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

IScreen* ScreenList::getCurScreen() const {
	if (m_currentScreenIndex == SCREEN_INDEX_NO_SCREEN) return nullptr;
	return m_screens[m_currentScreenIndex];
}
}
