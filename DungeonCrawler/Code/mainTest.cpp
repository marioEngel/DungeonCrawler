#include <iostream>
#include "Game.h"
#include "Math/Vector2D.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>

float gDeltaT{};
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gRenderertex_UI = NULL;
SDL_Texture* gFontTexture = NULL;

void initStuff()
{
	if (SDL_Init(SDL_INIT_VIDEO) && SDL_Init(SDL_INIT_EVENTS) && SDL_Init(SDL_INIT_AUDIO))
	{
		gWindow = SDL_CreateWindow("text", 800, 600, 0);
		gRenderer = SDL_CreateRenderer(gWindow, NULL);
		if (gRenderer)
		{
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		}
	}
	else {
		std::cout << "Error : " << SDL_GetError() << std::endl;
	}
	SDL_StartTextInput(gWindow);

	if (gRenderertex_UI == nullptr)
	{
		gRenderertex_UI = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 800, 600);
	}
}

SDL_Texture* initTTF()
{
	TTF_Init();

	TTF_Font* font = TTF_OpenFont("Font/joystix/joystix_monospace.otf", 32);
	SDL_Color color = { 255, 255, 255, 255 };

	SDL_Surface* surface = TTF_RenderText_Blended(font, "Test Text", 0, color);
	if (!surface) printf("Render error: %s\n", SDL_GetError());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);
	if (!texture) printf("Texture error: %s\n", SDL_GetError());

	SDL_DestroySurface(surface);

	return texture;
}

int main(int argc, char* arvg[])
{
	initStuff();
	gFontTexture = initTTF();

	int a =	0;


	SDL_SetRenderTarget(gRenderer, gRenderertex_UI);
	//SDL_RenderClear(gRenderer);
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderTexture(gRenderer, gFontTexture, NULL, NULL);
	SDL_SetRenderTarget(gRenderer, NULL);

	//SDL_RenderTexture(gRenderer, gFontTexture, NULL, NULL);

	while (true)
	{
		//SDL_SetRenderTarget(gRenderer, NULL);
		//SDL_RenderClear(gRenderer);
		//SDL_SetTextureBlendMode(gRenderertex_UI, SDL_BLENDMODE_BLEND);

		if (a == 1) {
			//SDL_RenderTexture(gRenderer, gFontTexture, NULL, NULL);
		}
		else
		{
			SDL_RenderTexture(gRenderer, gRenderertex_UI, NULL, NULL);
		}

		SDL_RenderPresent(gRenderer);
		SDL_RenderClear(gRenderer);
	}

	return 0;
}


//SDL_RenderClear(gRenderer);
//SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

//SDL_DestroySurface(surface);

//SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
//SDL_RenderClear(gRenderer);

//SDL_FRect dst = { 100, 100, 150, 50 };