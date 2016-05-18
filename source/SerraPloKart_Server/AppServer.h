#pragma once
#include <SerraPloEngine/IAppServer.h>
#pragma comment(lib, "SerraPloEngine.lib")
using namespace SerraPlo;

#define PORT 5000

class AppServer : public IAppServer {
	explicit AppServer() : IAppServer(PORT) {};
	AppServer(AppServer const&) = delete;
	void operator=(AppServer const&) = delete;
	~AppServer() = default;
public:
	static AppServer& Instance() {
		static AppServer instance;
		return instance;
	}
};

#undef IP_PORT