#include "TextureFunc.h"
#include "../../Game.h"

SDL_Texture* LoadTexture(const char* path)
{
	//SDL_RenderClear(Game::renderer);
	SDL_Surface* tmpSurface = IMG_Load(path);
	SDL_Texture* tmpTexture = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	return tmpTexture;
}

void drawAndClear()
{	
	SDL_RenderPresent(Game::renderer);
	SDL_RenderClear(Game::renderer);
}

void LoadTexture(const char* path, SDL_Rect& originRec, SDL_Rect& destRect)
{
	SDL_RenderClear(Game::renderer);
	SDL_Surface* tmpSurface = IMG_Load(path);
	SDL_Texture* tmpTexture = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	SDL_RenderCopy(Game::renderer, tmpTexture, &originRec, &destRect);
	SDL_RenderPresent(Game::renderer);
}