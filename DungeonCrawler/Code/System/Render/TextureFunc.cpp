#include "TextureFunc.h"
#include "../../Game.h"

SDL_Texture* LoadTexture(const char* path, SDL_Renderer* tmpRenderer)
{
	//SDL_RenderClear(Game::renderer);
	SDL_Surface* tmpSurface = IMG_Load(path);
	SDL_Texture* tmpTexture = SDL_CreateTextureFromSurface(tmpRenderer, tmpSurface);
	SDL_DestroySurface(tmpSurface);

	return tmpTexture;
}

SDL_Texture* LoadTextureText(TTF_Font* font, std::string text, SDL_Renderer* tmpRenderer, SDL_Color color)
{
	SDL_Surface* tmpSurface = TTF_RenderText_Blended(font, text.c_str(), 0, color);
	SDL_Texture* tmpTexture = SDL_CreateTextureFromSurface(tmpRenderer, tmpSurface);

	if (!tmpSurface) printf("Render error: %s\n", SDL_GetError());
	if (!tmpTexture) printf("Texture error: %s\n", SDL_GetError());

	SDL_DestroySurface(tmpSurface);

	return tmpTexture;
}

void drawAndClear(
	SDL_Texture* rendTex1,	// render tile map 
	SDL_Texture* rendTex2,  // render sprites
	SDL_Texture* rendTex3,  // render light
	SDL_Texture* rendTex4,  // render UI/text
	bool lightOn
)
{	
	SDL_SetRenderTarget(Game::renderer, NULL);
	SDL_RenderClear(Game::renderer);

	SDL_RenderTexture(Game::renderer, rendTex1, NULL, NULL);	
	SDL_RenderTexture(Game::renderer, rendTex2, NULL, NULL);
	if(!lightOn)
	{
		SDL_RenderTexture(Game::renderer, rendTex3, NULL, NULL);
	}
	SDL_RenderTexture(Game::renderer, rendTex4, NULL, NULL); 

	SDL_RenderPresent(Game::renderer);
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