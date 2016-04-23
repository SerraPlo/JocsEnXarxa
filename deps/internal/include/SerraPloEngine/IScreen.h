#pragma once

namespace SerraPlo {

	#define SCREEN_INDEX_NO_SCREEN -1

	enum class ScreenState { 
		NONE,
		RUNNING, 
		EXIT_APPLICATION,
		CHANGE_NEXT,
		CHANGE_PREVIOUS
	};

	class IApp;
	class IScreen
	{
	protected:
		ScreenState m_currentState = ScreenState::NONE;
		IApp* m_game = nullptr;
		int m_screenIndex = SCREEN_INDEX_NO_SCREEN;
	public:
		IScreen() = default;
		virtual ~IScreen() = default;

		//Called at beginning and end of app
		virtual void build() = 0;
		virtual void destroy() = 0;

		//Called when screen enters and exits focus
		virtual void onEntry() = 0;
		virtual void onExit() = 0;

		//Called in main game loop
		virtual void update() = 0;
		virtual void draw() = 0;

		//Return index of next or previous screen when changing screens
		virtual int getNextScreenIndex() const = 0;
		virtual int getPrevScreenIndex() const  = 0;
		int getScreenIndex() const { return m_screenIndex; };
		ScreenState getState() const { return m_currentState; };

		void setRunning() { m_currentState = ScreenState::RUNNING; };
		void setExit() { m_currentState = ScreenState::EXIT_APPLICATION; };
		void setParentGame(IApp* game) { m_game = game; };

		friend class ScreenList;
	};

}

