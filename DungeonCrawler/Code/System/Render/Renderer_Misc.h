#include <SDL3/SDL.h>

SDL_Texture* generateGaussianLight(SDL_Renderer* renderer, int width, int height, float sigma, float empty1, float empty2);
SDL_Texture* generateGaussianLightTwoSigma(SDL_Renderer* renderer, int width, int height, float sigmaX, float sigmaY, float empty2);
SDL_Texture* generateFlashLightCone(SDL_Renderer* renderer, int width, int height, float angle, float empty1, float empty2);
