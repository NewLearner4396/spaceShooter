#include <string>
#include "SceneTitle.h"
#include "SceneMain.h"
#include "Game.h"

void SceneTitle::init() {
    bgm = Mix_LoadMUS("../assets/music/06_Battle_in_Space_Intro.ogg");
    if (bgm == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load music: %s", SDL_GetError());
        return;
    }
    if (!Mix_PlayMusic(bgm, -1)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to play music: %s", SDL_GetError());
        return;
    }
}

void SceneTitle::update(float dT) {
    timer += dT;
    if (timer > 1.0f) {
        timer = 0;
    }
}

void SceneTitle::render() {
    std::string title = "Space Shooter";
    std::string text = "Press Enter to Start";
    game.renderTextureCentered(title, 0.45f, textType::Title);
    if (timer < 0.5f)
        game.renderTextureCentered(text, 0.6f, textType::Text);
}

void SceneTitle::clean() {
    if (bgm != nullptr) {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
        bgm = nullptr;
    }
}

void SceneTitle::handleEvent(SDL_Event* sdlEvent) {
    if (sdlEvent->type == SDL_EVENT_KEY_DOWN) {
        if (sdlEvent->key.scancode == SDL_SCANCODE_RETURN || sdlEvent->key.scancode == SDL_SCANCODE_KP_ENTER) {
            game.changeScene(new SceneMain());
        }
    }
    if (sdlEvent->type == SDL_EVENT_QUIT) {
        game.pause();
    }
}
