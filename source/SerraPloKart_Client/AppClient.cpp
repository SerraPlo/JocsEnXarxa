#include <SerraPloEngine/ScreenList.h>
#include <SerraPloEngine/ResourceManager.h>
#include "AppClient.h"
#include <SerraPloEngine/DebugPrimitives.h>

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

void AppClient::OnInit() {
	glClearDepth(1.0);			// Set the base depth when depth buffer
	glEnable(GL_DEPTH_TEST);	// Activate the Z-buffer
	glEnable(GL_BLEND);			// Enable alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	EnableGLHint();

	// Temp window for loading screen purposes
	SDL_Window *msgbox = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 100, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
	SDL_Renderer *msgboxR = SDL_CreateRenderer(msgbox, 0, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(msgboxR, 255, 255, 255, 255);
	SDL_Surface *text_surface = nullptr;
	SDL_RenderClear(msgboxR);
	TTF_Font *font = TTF_OpenFont(LoadAsset("fonts/ARIAL.TTF").c_str(), 40);
	if ((text_surface = TTF_RenderText_Blended(font, "Loading assets...", { 0,0,0 })) != nullptr) {
		SDL_Texture *texture = nullptr;
		if ((texture = SDL_CreateTextureFromSurface(msgboxR, text_surface)) != nullptr) {
			SDL_RenderCopy(msgboxR, texture, nullptr, nullptr);
			SDL_RenderPresent(msgboxR);
			SDL_DestroyTexture(texture);
		}
		SDL_FreeSurface(text_surface);
	}

	//std::thread(LoadGame).detach();
	window.changeName("SerraPlo Kart Client");
	gameObjectManager.Load(LoadAsset("gameObjects.json"));

	SDL_DestroyRenderer(msgboxR);
	SDL_DestroyWindow(msgbox);
}

void AppClient::AddScreens() {
	m_testScreen = std::make_unique<PlaygroundScreen>();
	m_screenList->AddScreen(m_testScreen.get());
	m_screenList->SetScreen(m_testScreen->screenIndex);
}

void AppClient::OnExit() {}

#undef TARGET_FPS