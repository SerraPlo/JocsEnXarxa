#include <SerraPloEngine/Window.h>
#include <SerraPloEngine/GLSLManager.h>
#include <SerraPloEngine/Camera2D.h>
#include <SerraPloEngine/SpriteBatch.h>
#include <SerraPloEngine/ResourceManager.h>
using namespace SerraPlo;

enum class GameState { PLAY, GAMEOVER, EXIT };

int main() {

	GameState gameState = GameState::PLAY;
	int screenWidth = 800, screenHeight = 700;
	GLWindow window(screenWidth, screenHeight, "SerraPlo Test");
	GLSLManager textureProgram;
	Camera2D camera(screenWidth, screenHeight);
	SpriteBatch mainSpriteBatch;

	SDL_Init(SDL_INIT_EVERYTHING);
	//tell we want a double buffered windows to avoid flickering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	window.create(ColorRGBA8{ 125, 125, 125, 255 });
	textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	textureProgram.addAttribute("vertexPosition");
	textureProgram.addAttribute("vertexColor");
	textureProgram.addAttribute("vertexUV");
	textureProgram.linkShaders();
	mainSpriteBatch.init();

	while (gameState != GameState::EXIT) {
		camera.setPosition({ 0,0 });
		camera.update();

		// Set the base depth to 1.0
		glClearDepth(1.0);
		// Clear the color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		textureProgram.bind();

		glActiveTexture(GL_TEXTURE0);
		auto textureUniform = textureProgram.getUniformLocation("mySampler");
		glUniform1i(textureUniform, 0);

		auto projectionUniform = textureProgram.getUniformLocation("P");
		auto projectionMatrix = camera.getCameraMatrix();
		glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

		mainSpriteBatch.begin();

		glm::vec4 destRect(0, 0, 50, 50);
		const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		static int textureID = ResourceManager::getTexture("Textures/circle.png").id;
		mainSpriteBatch.pushBatch(destRect, uvRect, textureID, 0.0f, ColorRGBA8{ 75, 75, 75, 255 });

		mainSpriteBatch.end();
		mainSpriteBatch.renderBatches();

		textureProgram.unbind();

		// Swap our buffer and draw everything to the screen!
		window.swapBuffer();
	}

	return 0;
}