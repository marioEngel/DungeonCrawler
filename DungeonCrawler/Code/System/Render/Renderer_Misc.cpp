#include "Renderer_Misc.h"
#include "../../Math/Vector2D.h"

SDL_Texture* generateGaussianLight(SDL_Renderer* renderer, int width, int height, float sigma, float empty1, float empty2)
{
    SDL_Surface* surface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);

    Uint32* pixels = (Uint32*)surface->pixels;
    int pitch = surface->pitch / sizeof(Uint32);

    // Center of the texture
    float centerX = width / 2.0f;
    float centerY = height / 2.0f;
    float sigmaSq = 2.0f * sigma * sigma;

    // Iterate over each pixel
    for (int y = 0; y < height; y++) 
    {
        for (int x = 0; x < width; x++) 
        {
            // Compute distance from the center
            float dx = x - centerX;
            float dy = y - centerY;
            float distanceSq = dx * dx + dy * dy;

            // Apply Gaussian function
            float intensity = exp(-distanceSq / sigmaSq) * 255.0f;
            Uint8 color = static_cast<Uint8>(intensity);

            // Store grayscale color (R = G = B = intensity, A = intensity)
            pixels[y * pitch + x] = SDL_MapSurfaceRGBA(surface, color, color, color, color);
        }
    }
    // Convert surface to texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    return texture;
}

SDL_Texture* generateGaussianLightTwoSigma(SDL_Renderer* renderer, int width, int height, float sigmaX, float sigmaY, float empty2)
{
    SDL_Surface* surface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);

    Uint32* pixels = (Uint32*)surface->pixels;
    int pitch = surface->pitch / sizeof(Uint32);

    // Center of the texture
    float centerX = width / 2.0f;
    float centerY = height / 2.0f;
    float sigmaSqX = 2.0f * sigmaX * sigmaX;
    float sigmaSqY = 2.0f * sigmaY * sigmaY;

    // Iterate over each pixel
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Compute distance from the center
            float dx = x - centerX;
            float dy = y - centerY;
            float distanceSqX = dx * dx;
            float distanceSqY = dy * dy;

            // Apply Gaussian function
            float intensity = exp(-(distanceSqX / sigmaSqX + distanceSqY / sigmaSqY)) * 255.0f;
            Uint8 color = static_cast<Uint8>(intensity);

            // Store grayscale color (R = G = B = intensity, A = intensity)
            pixels[y * pitch + x] = SDL_MapSurfaceRGBA(surface, color, color, color, color);
        }
    }
    // Convert surface to texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    return texture;
}

SDL_Texture* generateFlashLightCone(SDL_Renderer* renderer, int width, int height, float angle, float empty1, float empty2)
{
    SDL_Surface* surface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);

    Uint32* pixels = (Uint32*)surface->pixels;
    int pitch = surface->pitch / sizeof(Uint32);

    // Center of the texture
    float centerX = width / 2.0f;
    float centerY = height / 2.0f;
    Vector2D<float> centerVec{ centerX, centerY };

    // Iterate over each pixel
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Compute distance from the center
            float dx = x - centerX;
            float dy = y - centerY;
            float distanceSq = dx * dx + dy * dy;
            float distance = sqrt(distanceSq);

            // Apply Gaussian function
            float intensity = 0;
            Vector2D<float> tmpVec{ float(x), float(y) };
            float tmpAngle = calc_angle(centerVec, tmpVec, false) * 180 / M_PI;
            if (tmpAngle < angle / 2.0f || tmpAngle > 360.0f - angle / 2.0f)
            {
                intensity = exp(-distance / 500) * 255.0f;
            }
            Uint8 color = static_cast<Uint8>(intensity);

            // Store grayscale color (R = G = B = intensity, A = intensity)
            pixels[y * pitch + x] = SDL_MapSurfaceRGBA(surface, color, color, color, color);
        }
    }
    // Convert surface to texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    return texture;
}

float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

float hash(int i) {
    i = (i << 13) ^ i;
    return (1.f - ((i * (i * i * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.f);
}

float valueNoise(float t) {
    int i = (int)t;
    float f = t - i;
    f = f * f * (3.0f - 2.0f * f);
    return lerp(hash(i), hash(i + 1), f);
}