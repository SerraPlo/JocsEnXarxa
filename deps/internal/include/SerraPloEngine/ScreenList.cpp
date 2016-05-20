#include "ScreenList.h"
#include "IScreen.h"

namespace SerraPlo {

	ScreenList::ScreenList(IApp* game) :
		m_gameApp(game),
		m_currentScreenIndex(SCREEN_INDEX_NO_SCREEN){}

	ScreenList::~ScreenList() {
		//Destroy();
	}

	IScreen* ScreenList::MoveNext() {
		auto currentScreen = GetCurScreen();
		if (currentScreen->GetNextScreenIndex() != SCREEN_INDEX_NO_SCREEN) // Check if the next screen in list exists
			m_currentScreenIndex = currentScreen->GetNextScreenIndex(); // Then move to the next screen by changing the index
		return GetCurScreen(); // Return the new screen
	}

	IScreen* ScreenList::MovePrev() {
		auto currentScreen = GetCurScreen();
		if (currentScreen->GetPrevScreenIndex() != SCREEN_INDEX_NO_SCREEN) // Check if the previous screen in list exists
			m_currentScreenIndex = currentScreen->GetPrevScreenIndex(); // Then move to the previous screen by changing the index
		return GetCurScreen(); // Return the new screen
	}

	void ScreenList::SetScreen(int nextScreen) {
		m_currentScreenIndex = nextScreen;
	}

	void ScreenList::AddScreen(IScreen* newScreen) {
		newScreen->screenIndex = m_screens.size(); // Assign the size of the list as the index of the new screen
		m_screens.push_back(newScreen); // Add the new screen into the list
		newScreen->gameApp = m_gameApp; // Set the game app reference parent of the new screen
		newScreen->Build(); // Initialize the new screen
	}

	IScreen* ScreenList::GetCurScreen() const {
		if (m_currentScreenIndex == SCREEN_INDEX_NO_SCREEN) return nullptr; // Check if current screen exists
		return m_screens[m_currentScreenIndex]; // If exists, return the current running screen
	}

	void ScreenList::Destroy() {
		for (auto s : m_screens) s->Destroy(); // Remove elements of the screen and destroy the screen
		m_screens.resize(0); // Keep the capacity of the list removing the elements whithout deallocating memory
		m_currentScreenIndex = SCREEN_INDEX_NO_SCREEN; // Set current screen index to -1
	}

}
