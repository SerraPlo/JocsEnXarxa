#pragma once
#include <SerraPloEngine/IAppClient.h>
#include "PlaygroundScreen.h"

class AppClient : public IAppClient {
	std::unique_ptr<PlaygroundScreen> m_testScreen { nullptr };
public:
	explicit AppClient(const char* ipport) : IAppClient(ipport) {};
	~AppClient() = default;

	virtual void OnInit() override;
	virtual void AddScreens() override;
	virtual void OnExit() override;
};

