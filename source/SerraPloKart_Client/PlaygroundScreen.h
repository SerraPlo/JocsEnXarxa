#pragma once
#include <SerraPloEngine/Renderer3D.h>
#include <SerraPloEngine/IScreen.h>
#include <SerraPloEngine/Camera3D.h>
#include <SerraPloEngine/ShaderProgram.h>
#include <SOIL/SOIL.h>

#pragma comment(lib, "SOIL.lib")
#pragma comment(lib, "SerraPloEngine.lib")

using namespace SerraPlo;

class PlaygroundScreen : public IScreen
{
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

	ShaderProgram m_shaderProgram;
	Camera3D m_camera;
	GLMesh testMesh;
};
