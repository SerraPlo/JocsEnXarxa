#pragma once
#include <SerraPloEngine/IAppClient.h>
#include "PlaygroundScreen.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define IP_PORT "127.0.0.1:5000"

class AppClient : public IAppClient {
	std::unique_ptr<PlaygroundScreen> m_testScreen { nullptr };

	explicit AppClient() : IAppClient(SCREEN_WIDTH, SCREEN_HEIGHT, IP_PORT) {};
	AppClient(AppClient const&) = delete;
	void operator=(AppClient const&) = delete;
	~AppClient() = default;
public:
	static AppClient& Instance() {
		static AppClient instance;
		return instance;
	}

	virtual void OnInit() override;
	virtual void AddScreens() override;
	virtual void OnExit() override;
};
#undef IP_PORT