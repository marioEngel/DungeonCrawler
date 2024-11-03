#include "TextureFunc.h"
#include "../../Game.h"

SDL_Texture* LoadTexture(const char* path, SDL_Renderer* tmpRenderer)
{
	//SDL_RenderClear(Game::renderer);
	SDL_Surface* tmpSurface = IMG_Load(path);
	SDL_Texture* tmpTexture = SDL_CreateTextureFromSurface(tmpRenderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	return tmpTexture;
}

SDL_Texture* LoadTexture2(const char* path, SDL_Renderer* tmpRenderer)
{
	//SDL_RenderClear(Game::renderer);
	SDL_Surface* tmpSurface = IMG_Load(path);
	SDL_Texture* tmpTexture = SDL_CreateTextureFromSurface(tmpRenderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);

	SDL_SetTextureBlendMode(tmpTexture, SDL_BLENDMODE_ADD);
	std::cout << "here\n";
	return tmpTexture;
}

void drawAndClear(SDL_Texture* rendTex1, SDL_Texture* rendTex2, SDL_Texture* rendTex3)
{	
	SDL_SetRenderTarget(Game::renderer, NULL);
	SDL_RenderClear(Game::renderer);

	SDL_SetTextureBlendMode(rendTex1, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(rendTex2, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(rendTex3, SDL_BLENDMODE_MOD);

	SDL_RenderCopy(Game::renderer, rendTex1, NULL, NULL);
	SDL_RenderCopy(Game::renderer, rendTex2, NULL, NULL);
	SDL_RenderCopy(Game::renderer, rendTex3, NULL, NULL);

	SDL_RenderPresent(Game::renderer);
	SDL_RenderClear(Game::renderer);
}

/*
void LoadTexture(const char* path, SDL_Rect& originRec, SDL_Rect& destRect, SDL_Renderer* tmpRenderer)
{
	SDL_RenderClear(tmpRenderer);
	SDL_Surface* tmpSurface = IMG_Load(path);
	SDL_Texture* tmpTexture = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	SDL_RenderCopy(tmpRenderer, tmpTexture, &originRec, &destRect);
	SDL_RenderPresent(tmpRenderer);
} 
 */