#include "RendererTileMap.h"
#include "../../ECS/Coordinator.h"
#include "Misc/TextureFunc.h"
#include "../../Component/Comp_TileMap.h"
#include "../../Game.h"
#include "../Collision/CollisionFunc.h"
#include "Camera/Camera.h"

extern Camera gCamera;
extern Coordinator gCoordinator;

void SysRendererTileMap::initRenderertex() {
    if (renderertex_map == nullptr) {
        renderertex_map = SDL_CreateTexture(
            Game::renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            Game::screenWidth,
            Game::screenHeight
        );
        SDL_SetTextureBlendMode(renderertex_map, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderTarget(Game::renderer, renderertex_map);
    SDL_RenderClear(Game::renderer);
    SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 0);
}


// load the textures once
void SysRendererTileMap::loadTexture() {
    for (auto const &entity: mEntities) {
        auto &tileMap = gCoordinator.GetComponent<TileMap>(entity);

        if (tileMap.textures.empty()) {
            for (int i = 0; i < tileMap.paths.size(); i++) {
                tileMap.textures.emplace_back(LoadTexture(tileMap.paths[i], Game::renderer));
            }
        }
    }
}

void SysRendererTileMap::createTileMapRenderertex(bool forceUpdate) {
    for (auto const &entity: mEntities) {
        auto &tileMap = gCoordinator.GetComponent<TileMap>(entity);

        if (tileMap.tileMapTexture == nullptr || forceUpdate) {
            const int tileMapWidth = tileMap.matrix.width();
            const int tileMapHight = tileMap.matrix.height();
            const int texWidth = static_cast<int>(tileMapWidth * tileMap.cubeSize);
            const int texHeight = static_cast<int>(tileMapHight * tileMap.cubeSize);

            tileMap.dimension = Vector2D<int>{texWidth, texHeight};

            tileMap.tileMapTexture = SDL_CreateTexture(
                Game::renderer, SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET, texWidth, texHeight);
            SDL_SetRenderTarget(Game::renderer, tileMap.tileMapTexture);
            SDL_RenderClear(Game::renderer);
            SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 0);

            SDL_FRect srcRec{0.0f, 0.0f, 32.0f, 32.0f};

            for (int xxx = 0; xxx < tileMapWidth; xxx++) {
                for (int yyy = 0; yyy < tileMapHight; yyy++) {
                    SDL_FRect destRec{
                        tileMap.cubeSize * float(xxx), tileMap.cubeSize * float(yyy), tileMap.cubeSize, tileMap.cubeSize
                    };
                    SDL_RenderTexture(Game::renderer, tileMap.textures[tileMap.matrix(xxx, yyy)], &srcRec, &destRec);
                }
            }
        }
    }
    SDL_SetRenderTarget(Game::renderer, nullptr);
}

void SysRendererTileMap::render() {
    for (auto const &entity: mEntities) {
        auto &tileMap = gCoordinator.GetComponent<TileMap>(entity);

        SDL_FRect src{0, 0, float(tileMap.dimension[0]), float(tileMap.dimension[1])};
        SDL_FRect dst{-gCamera.mCamera.x, -gCamera.mCamera.y, float(tileMap.dimension[0]), float(tileMap.dimension[1])};
        SDL_RenderTexture(Game::renderer, tileMap.tileMapTexture, &src, &dst);
    }
    SDL_SetRenderTarget(Game::renderer, nullptr);
}

SDL_Texture *SysRendererTileMap::rtnRenderertex() {
    return renderertex_map;
}
