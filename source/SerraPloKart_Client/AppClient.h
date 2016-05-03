#pragma once
#include <SerraPloEngine/IAppClient.h>
#include <SerraPloEngine/EntityManager.h>
#include "PlaygroundScreen.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define IP_PORT "127.0.0.1:5000"

//(modelos / texturas)

//gameobject(modelo,textura)

//hashmap<k=(modelo,textura),v=vbo(T)>

/*coche, coche, coche, avion, avion
coche, avion, coche, avion, coche

setuniform(camera)
for (auto...) {
	setvao(modelo)
	settexture(textura)
	for (model : .second) {
		setuniform(model / transform)
		drawelements()
	}
}*/

class AppClient : public IAppClient {
	EntityManager m_entityManager;
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

#undef SCREEN_WIDTH
#undef SCREEN_HEIGHT
#undef IP_PORT