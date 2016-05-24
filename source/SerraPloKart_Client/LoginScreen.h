#pragma once
#include <string>
#include <SerraPloEngine\IScreen.h>
#include <SerraPloEngine\Sprite.h>
#pragma comment(lib, "SerraPloEngine.lib")
using namespace SerraPlo;

#define TEXT_WIDTH 700
#define TEXT_HEIGHT 100
#define TEXT_MAX_SIZE 10

class AppClient;
class LoginScreen : public IScreen{
public:
	explicit LoginScreen() = default;
	~LoginScreen() = default;

	void Build(void) override;
	void Destroy(void) override;
	void OnEntry(void) override;
	void OnExit(void) override;
	void Update(void) override;
	void Draw(void) override;
private:
	AppClient *m_app; // Casted client main game pointer reference (IApp -> AppClient)
	std::string m_nickText{ " " };
	// Text to show on screen
	Text m_loginMessage;
	Text m_loadMessage;
	InputText m_userInput;
	bool loading{ false }; ///TODO: loading screen not cutre
};

