#include "KeyboardInput.h"
#include "../../Game.h"
#include <SDL3/SDL.h>

KeyboardInput::KeyboardInput() {
    memset(prevKeys, 0, SDL_SCANCODE_COUNT);
    currKeys = SDL_GetKeyboardState(nullptr);
}

void KeyboardInput::registerInput() {
    SDL_PollEvent(&Game::event);
    currKeys = SDL_GetKeyboardState(nullptr);
}

void KeyboardInput::getPrevKeys() {
    memcpy(prevKeys, currKeys, SDL_SCANCODE_COUNT);
}

eButtonState KeyboardInput::getButtonState(const int number) {
    if (prevKeys[number] == 0) {
        if (currKeys[number] == 1) {
            return eButtonState::PRESSED;
        } else {
            return eButtonState::NONE;
        }
    } else {
        if (currKeys[number] == 1) {
            return eButtonState::HELD;
        } else {
            return eButtonState::RELEASED;
        }
    }
}
