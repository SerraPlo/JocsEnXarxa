#pragma once
#include <SerraPloEngine/IScreen.h>
#include <SerraPloEngine/ShaderProgram.h>
#include <SerraPloEngine/Camera.h>
#include <SerraPloEngine/GameObject.h>
#include <SerraPloEngine/Light.h>
#include "SceneObjectList.h"
#pragma comment(lib, "SerraPloEngine.lib")
using namespace SerraPlo;

class PlaygroundScreen : public IScreen {
public:
	explicit PlaygroundScreen();
	~PlaygroundScreen();

	virtual void Build() override;
	virtual void Destroy() override;
	virtual void OnEntry() override;
	virtual void OnExit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual int GetNextScreenIndex() const override;
	virtual int GetPrevScreenIndex() const override;
private:
	void checkInput();

	ShaderProgram m_mainProgram;
	ShaderProgram m_lightProgram;
	Camera m_camera;
	SceneObjectList m_sceneObjects;
	GameObject *m_player;
	GLuint VBO, containerVAO;
};
