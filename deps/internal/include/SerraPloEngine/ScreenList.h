#pragma once
#include <vector>

namespace SerraPlo {

	class IAppClient;	// Forward declaration of the main game app interface
	class IScreen;		// Forward declaration of the game screen interface

	// Screen list class to store the whole game screens
	class ScreenList {
	protected:
		std::vector<IScreen*> m_screens;	// Array of screens
		IAppClient* m_gameApp;				// Reference pointer to current game app	
		int m_currentScreenIndex;			// Index of the current running screen
	public:
		explicit ScreenList(IAppClient* game);
		~ScreenList();

		// Destroy each screen inside the list
		void Destroy();

		// Move to the next screen from the current one
		IScreen* MoveNext();
		// Move to the previous screen from the current one
		IScreen* MovePrev();

		// Set the current screen index
		void SetScreen(int nextScreen);
		// Add new screen to array
		void AddScreen(IScreen* newScreen);

		// Get the current screen object
		IScreen* GetCurScreen() const;
	};

}