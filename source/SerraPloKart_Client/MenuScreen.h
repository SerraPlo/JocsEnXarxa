#pragma once
#include <SerraPloEngine\IScreen.h>
#include <SerraPloEngine\Sprite.h>
#pragma comment(lib, "SerraPloEngine.lib")
using namespace SerraPlo;

class AppClient;
class MenuScreen : public IScreen {
public:
	explicit MenuScreen() = default;
	~MenuScreen() = default;

	void Build(void) override;
	void Destroy(void) override;
	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	AppClient *m_app; // Casted client main game pointer reference (IApp -> AppClient)
	// Sprites to load
	Sprite m_title;
	Button m_singlePlayer;
};

