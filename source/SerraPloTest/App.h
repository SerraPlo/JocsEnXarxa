#pragma once
#include <SerraPloEngine/IApp.h>
#include "PlaygroundScreen.h"

class App : public SerraPlo::IApp
{
	std::unique_ptr<PlaygroundScreen> m_testScreen = nullptr;
public:
	App();
	~App();

	virtual void onInit() override;
	virtual void addScreens() override;
	virtual void onExit() override;
};

