#pragma once
#include <SerraPloEngine/IApp.h>
#include "PlaygroundScreen.h"

#define TARGET_FPS 120.0f

class App : public SerraPlo::IApp
{
	std::unique_ptr<PlaygroundScreen> m_testScreen = nullptr;
public:
	App();
	~App();

	virtual void OnInit() override;
	virtual void AddScreens() override;
	virtual void OnExit() override;
};

