#include <string>
#include <map>
#include "SceneEnd.h"
#include "SceneTitle.h"
#include "SceneMain.h"
#include "Game.h"

void SceneEnd::init() {
    bgm = Mix_LoadMUS("../assets/music/06_Battle_in_Space_Intro.ogg");
    if (bgm == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load music: %s", SDL_GetError());
        return;
    }
    if (!Mix_PlayMusic(bgm, -1)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to play music: %s", SDL_GetError());
        return;
    }
    if(!SDL_TextInputActive(game.getWindow())) {
        SDL_StartTextInput(game.getWindow());
        isTyping = true;
    }
    if (!SDL_TextInputActive(game.getWindow())) {
        isTyping = false;
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to start text input: %s", SDL_GetError());
    }
}

void SceneEnd::clean() {
    if (bgm != nullptr) {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
        bgm = nullptr;
    }
    if (isTyping) {
        SDL_StopTextInput(game.getWindow());
    }
    playerName.clear();
}

void SceneEnd::update(float deltaTime) {
    textBlinkTimer += deltaTime;
    if (textBlinkTimer > textBlinkDelay*2) {
        textBlinkTimer = 0;
    }
    cursorBlinkTimer += deltaTime;
    if (cursorBlinkTimer > cursorBlinkDelay*2) {
        cursorBlinkTimer = 0;
    }
}

void SceneEnd::render() {
    if (isTyping) {
        std::string textScore = "Your score: " + std::to_string(game.getFinalScore());
        std::string textGameOver = "Game Over!";
        std::string textInput = "Please input your name:";
        std::string textInput2 = "Press Enter to confirm!";
        std::string textSkip = "Press TAB to skip!";
        game.renderTextureCentered(textGameOver, 0.2f, textType::Title);
        game.renderTextureCentered(textScore, 0.4f, textType::Text);
        game.renderTextureCentered(textInput, 0.6f, textType::Text);
        game.renderTextureCentered(textInput2, 0.75f, textType::Text);
        if (textBlinkTimer < textBlinkDelay) {
            game.renderTextureCentered(textSkip, 0.8f, textType::Text);
        }
        float nameY = 0.65f;
        if (cursorBlinkTimer < cursorBlinkDelay) {
            if (playerName.empty()) {
                playerName = "_";
            }
            else {
                playerName += "_";
            }
            game.renderTextureCentered(playerName, nameY, textType::Text);
            playerName.pop_back();
        }
        else {
            if(!playerName.empty()) {
                playerName += " ";
                game.renderTextureCentered(playerName, nameY, textType::Text);
                playerName.pop_back();
            }
            else {
                game.renderTextureCentered(" ", nameY, textType::Text);
            }
        }
    }
    else {
        game.renderTextureCentered("Rank", 0.05f, textType::Title);
        // initial a map to store player names
        std::map<std::string, int> playerList;
        float posY = 0.15f * game.getWindowHeight();
        int i = 0;
        for (auto iter = game.getRankBoard().begin(); iter != game.getRankBoard().end() && i < 8; ++iter) {
            if (i >= game.getRankBoard().size()) {
                break;
            }
            if (playerList.find(iter->second) == playerList.end()) {
                playerList[iter->second] = iter->first;
            }else
                continue;
            std::string nameText = std::to_string(i + 1) + ". " + iter->second;
            game.renderTexturePos(nameText, {100, posY + i * 50}, textType::Text, true);
            std::string scoreText = std::to_string(iter->first);
            game.renderTexturePos(scoreText, {100, posY + i * 50}, textType::Text, false);
            i++;
        }
        game.renderTextureCentered("Your Score: " + std::to_string(game.getFinalScore()), 0.7f, textType::Text);
        if (textBlinkTimer < textBlinkDelay) {
            game.renderTextureCentered("Press TAB to play again!", 0.8f, textType::Text);
        }
    }
}

void SceneEnd::handleEvent(SDL_Event* sdlEvent) {
    if (isTyping) {
        if (sdlEvent->type == SDL_EVENT_TEXT_INPUT) {
                playerName += sdlEvent->text.text;
            }
        if (sdlEvent->type == SDL_EVENT_KEY_DOWN) {
            if (sdlEvent->key.scancode == SDL_SCANCODE_BACKSPACE) {
                if (!playerName.empty()) {
                    removeLastUTF8Char(playerName);
                }
            }
            else if (sdlEvent->key.scancode == SDL_SCANCODE_RETURN || sdlEvent->key.scancode == SDL_SCANCODE_KP_ENTER) {
                isTyping = false;
                SDL_StopTextInput(game.getWindow());
                if (playerName.empty()) {
                    playerName = "Anonymous Player";
                }
                game.insertRankBoard(game.getFinalScore(), playerName);
            }
            else if (sdlEvent->key.scancode == SDL_SCANCODE_TAB) {
                isTyping = false;
                SDL_StopTextInput(game.getWindow());
                game.changeScene(new SceneMain());
            }
        }
    }
    else {
        if (sdlEvent->type == SDL_EVENT_KEY_DOWN) {
            if (sdlEvent->key.scancode == SDL_SCANCODE_TAB) {
                game.changeScene(new SceneMain());
            }
            else if (sdlEvent->key.scancode == SDL_SCANCODE_ESCAPE) {
                game.changeScene(new SceneTitle());
            }
        }
    }
}

void SceneEnd::removeLastUTF8Char(std::string& str) {
    if (str.empty()) {
        return;
    }
    auto lastChar = str.back();
    if ((lastChar & 0b10000000) == 0b10000000) { // Check if it's a continuation byte character
        str.pop_back();
        while (str.length() > 0 && (str.back() & 0b11000000) != 0b11000000) { // Check if it's a multi-byte character
            str.pop_back();
        }
    }
    str.pop_back();
}
