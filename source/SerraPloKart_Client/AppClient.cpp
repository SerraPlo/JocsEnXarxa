#include <SerraPloEngine/ScreenList.h>
#include <SerraPloEngine/ResourceManager.h>
#include "AppClient.h"
#include <thread>
#include <atomic>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL_ttf.h>
#pragma commnent(lib, "SDL2_ttf.lib")

static void EnableGLHint() {
	//Indicates the accuracy of fog calculation.If per - pixel fog calculation is not efficiently supported by the GL implementation, 
	//hinting GL_DONT_CARE or GL_FASTEST can result in per - vertex calculation of fog effects.	
	glHint(GL_FOG_HINT, GL_NICEST);
	//Indicates the accuracy of the derivative calculation for the GL shading language fragment processing built - in functions : dFdx, dFdy, and fwidth.
	glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_NICEST);
	//Indicates the quality of filtering when generating mipmap images.
	glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
	//Indicates the sampling quality of antialiased lines.If a larger filter function is applied, hinting GL_NICEST can result in more pixel fragments being generated during rasterization.
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	//Indicates the quality of color, texture coordinate, and fog coordinate interpolation.If perspective - corrected parameter interpolation is not efficiently supported by the GL implementation, 
	//hinting GL_DONT_CARE or GL_FASTEST can result in simple linear interpolation of colors and / or texture coordinates.
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//Indicates the sampling quality of antialiased points.If a larger filter function is applied, hinting GL_NICEST can result in more pixel fragments being generated during rasterization.
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	//Indicates the sampling quality of antialiased polygons.Hinting GL_NICEST can result in more pixel fragments being generated during rasterization, if a larger filter function is applied.
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	//Indicates the quality and performance of the compressing texture images. Hinting GL_FASTEST indicates that texture images should be compressed as quickly as possible, 
	//while GL_NICEST indicates that texture images should be compressed with as little image quality loss as possible. 
	//GL_NICEST should be selected if the texture is to be retrieved by glGetCompressedTexImage for reuse.
	glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);
}

static std::atomic<bool> loading = true;

static void LoadGame() {
	while (loading) {
		std::cout << "Loading assets..." << std::endl;
		/*m_textProgram->bind();
		glUniform3f(m_textProgram->getUniformLocation("textColor"), 255, 0, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(*VAO);

		GLfloat vertices[6][4] = {
			{ 0,     0 + 1,   0.0, 0.0 },
			{ 0,     0,       0.0, 1.0 },
			{ 0 + 1, 0,       1.0, 1.0 },

			{ 0,     0 + 1,   0.0, 0.0 },
			{ 0 + 1, 0,       1.0, 1.0 },
			{ 0 + 1, 0 + 1,   1.0, 0.0 }
		};
		//glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, *VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_textProgram->unbind();

		window->swapBuffer();*/
	}
}

void AppClient::OnInit() {
	glClearDepth(1.0);			// Set the base depth when depth buffer
	glEnable(GL_DEPTH_TEST);	// Activate the Z-buffer
	glEnable(GL_BLEND);			// Enable alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	EnableGLHint();

	/*ShaderProgram m_textProgram;
	m_textProgram.LoadShaders(LoadAsset("shaders/text.vert"), LoadAsset("shaders/text.frag"));

	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(SCREEN_WIDTH), 0.0f, static_cast<GLfloat>(SCREEN_HEIGHT));
	glUniformMatrix4fv(m_textProgram.getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);*/

	std::thread(LoadGame).detach();
	window.changeName("SerraPlo Kart Client");
	gameObjectManager.Load(LoadAsset("gameObjects.json"));
	loading = false;
}

void AppClient::AddScreens() {
	m_testScreen = std::make_unique<PlaygroundScreen>();
	m_screenList->AddScreen(m_testScreen.get());
	m_screenList->SetScreen(m_testScreen->screenIndex);
}

void AppClient::OnExit() {}

#undef TARGET_FPS