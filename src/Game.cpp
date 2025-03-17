#include "Game.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "Scene.h"
#include "SceneMain.h"
#include "SceneTitle.h"

Game::Game() {}

Game::~Game() {
    saveRankBoard(rankBoardFileName, rankBoard);
    clean();
}

void Game::init() {
    // SDL initialization
    if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init Error: %s", SDL_GetError());
        return;
    }
    // Create window
    window = SDL_CreateWindow("SHOOTER", static_cast<int>(windowWidth), static_cast<int>(windowHeight), 0);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindow Error: %s", SDL_GetError());
        return;
    }
    // Create renderer
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateRenderer Error: %s", SDL_GetError());
        return;
    }
    SDL_SetRenderLogicalPresentation(renderer, static_cast<int>(windowWidth), static_cast<int>(windowHeight),
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);
    // SDL3_image does not need to be initialized
    // SDL3_ttf initialization
    if (!TTF_Init()) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_Init Error: %s", SDL_GetError());
        return;
    }
    // Load fonts
    titleFont = TTF_OpenFont("../assets/font/VonwaonBitmap-16px.ttf", 64);
    textFont = TTF_OpenFont("../assets/font/VonwaonBitmap-16px.ttf", 32);
    if (!titleFont || !textFont) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_OpenFont Error: %s", SDL_GetError());
        return;
    }
    // SDL_mixer initialization
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_MP3 | MIX_INIT_OGG)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_mixer could not initialize! SDL_mixer Error: %s\n", SDL_GetError());
        return;
    }
    // Open audio device
    if (!Mix_OpenAudio(0, NULL)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_mixer could not open audio! SDL_mixer Error: %s\n", SDL_GetError());
        return;
    }
    // Set the number of sound effect channels
    Mix_AllocateChannels(32);
    // Set music volume
    Mix_VolumeMusic(MIX_MAX_VOLUME / 10);
    Mix_Volume(-1, MIX_MAX_VOLUME / 8);

    nearStars = new Background();
    nearStars->texture = IMG_LoadTexture(renderer, "../assets/image/Stars-A.png");
    if (!nearStars->texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load nearStars texture: %s", SDL_GetError());
        return;
    }
    SDL_GetTextureSize(nearStars->texture, &nearStars->width, &nearStars->height);
    nearStars->width /= 2;
    nearStars->height /= 2;
    farStars = new Background();
    farStars->texture = IMG_LoadTexture(renderer, "../assets/image/Stars-B.png");
    if (!farStars->texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load farStars texture: %s", SDL_GetError());
        return;
    }
    SDL_GetTextureSize(farStars->texture, &farStars->width, &farStars->height);
    farStars->width /= 2;
    farStars->height /= 2;
    farStars->speed = 10;

    // Load rank board
    loadRankBoard(rankBoardFileName);

    // Initialize the first scene
    currentScene = new SceneTitle();
    currentScene->init();
    isRunning = true;
}
/**
 * @brief The main game loop.
 * 
 */
void Game::run() {
    while (exeRunning) {
        while (isRunning) {
            auto start = SDL_GetTicks();
            handleEvent(&event);
            update(deltaTime);
            render();
            auto end = SDL_GetTicks();
            auto diff = end - start;
            if (diff < frameTime) {
                SDL_Delay(static_cast<Uint32>(frameTime - diff));
                deltaTime = frameTime / 1000.0f;
            } else {
                deltaTime = diff / 1000.0f;
            }
        }
        if (!exeRunning) {
            break;
        }
        while (!isRunning) {
            SDL_PollEvent(&event);
            if (event.type == SDL_EVENT_QUIT) {
                exeRunning = false;
                return;
            } else if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.scancode == SDL_SCANCODE_RETURN || event.key.scancode == SDL_SCANCODE_KP_ENTER) {
                    isRunning = true;
                } else if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
                    changeScene(new SceneTitle());
                }
            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    isRunning = true;
                }
            }
        }
    }
}

void Game::pause() { isRunning = false; }

void Game::resume() { isRunning = true; }

void Game::stop() { changeScene(new SceneTitle()); }

void Game::clean() {
    // Clean up current scene
    if (currentScene) {
        currentScene->clean();
        delete currentScene;
        currentScene = nullptr;
    }
    // Clean up background textures
    if (nearStars) {
        SDL_DestroyTexture(nearStars->texture);
        delete nearStars;
        nearStars = nullptr;
    }
    if (farStars) {
        SDL_DestroyTexture(farStars->texture);
        delete farStars;
        farStars = nullptr;
    }
    // Clean up fonts
    if (titleFont) {
        TTF_CloseFont(titleFont);
        titleFont = nullptr;
    }
    if (textFont) {
        TTF_CloseFont(textFont);
        textFont = nullptr;
    }
    TTF_Quit();
    // SDL3_mixer cleanup
    Mix_CloseAudio();
    Mix_Quit();
    // Destroy renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::changeScene(Scene* scene) {
    // Clean up current scene and switch to new scene
    if (currentScene) {
        currentScene->clean();
        delete currentScene;
    }
    currentScene = scene;
    currentScene->init();
}

void Game::handleEvent(SDL_Event* sdlEvent) {
    while (SDL_PollEvent(sdlEvent)) {
        if (sdlEvent->type == SDL_EVENT_QUIT) {
            isRunning = false;
            exeRunning = false;
            return;
        }
        if (sdlEvent->type == SDL_EVENT_KEY_DOWN) {
            if (sdlEvent->key.scancode == SDL_SCANCODE_F4) {
                isFullScreen = !isFullScreen;
                SDL_SetWindowFullscreen(window, isFullScreen ? SDL_WINDOW_FULLSCREEN : 0);
            }
        }
        currentScene->handleEvent(sdlEvent);
    }
}

void Game::update(float dT) {
    currentScene->update(dT);
    backgroundUpdate(dT);
}

void Game::render() {
    SDL_RenderClear(renderer);
    currentScene->render();
    backgroundRender();
    SDL_RenderPresent(renderer);
}

void Game::renderTextureCentered(std::string text, float posY, textType type) {
    SDL_Color textColor = {255, 255, 255, 255};
    TTF_Font* font = (type == textType::Title) ? titleFont : textFont;
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), 0, textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FRect textRect = {windowWidth / 2 - surface->w / 2, (windowHeight - surface->h) * posY,
                          static_cast<float>(surface->w), static_cast<float>(surface->h)};
    SDL_RenderTexture(renderer, texture, NULL, &textRect);
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
}

void Game::renderTexturePos(std::string text, SDL_FPoint pos, textType type, bool isLeft) {
    SDL_Color textColor = {255, 255, 255, 255};
    TTF_Font* font = (type == textType::Title) ? titleFont : textFont;
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), 0, textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FRect textRect;
    if (isLeft) {
        textRect = {pos.x, pos.y, static_cast<float>(surface->w), static_cast<float>(surface->h)};
    } else {
        textRect = {windowWidth - pos.x - surface->w, pos.y, static_cast<float>(surface->w),
                    static_cast<float>(surface->h)};
    }
    SDL_RenderTexture(renderer, texture, NULL, &textRect);
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
}

void Game::saveRankBoard(std::string fileName, std::multimap<int, std::string, std::greater<int>>& rankBoard_) {
    std::ofstream file(fileName);
    if (!file.is_open()) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to open file: %s", fileName.c_str());
        return;
    }
    for (const auto& entry : rankBoard_) {
        file << entry.first << " " << entry.second << "\n";
    }
    file.close();
}

void Game::loadRankBoard(std::string fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to open file: %s", fileName.c_str());
        return;
    }
    rankBoard.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int score;
        std::string name;
        if (!(iss >> score >> name)) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to parse line: %s", line.c_str());
            continue;
        }
        rankBoard.insert(std::make_pair(score, name));
    }
    file.close();
    if (rankBoard.size() > rankBoardMaxSize) {
        auto it = rankBoard.begin();
        std::advance(it, rankBoardMaxSize);
        rankBoard.erase(it, rankBoard.end());
    }
}

void Game::backgroundUpdate(float dT) {
    nearStars->yOffSet += nearStars->speed * dT;
    if (nearStars->yOffSet >= 0) {
        nearStars->yOffSet -= nearStars->height;
    }
    farStars->yOffSet += farStars->speed * dT;
    if (farStars->yOffSet >= 0) {
        farStars->yOffSet -= farStars->height;
    }
}

void Game::backgroundRender() {
    for (float y = farStars->yOffSet; y < windowHeight; y += farStars->height) {
        for (float x = 0; x < windowWidth; x += farStars->width) {
            SDL_FRect dst = {x, y, farStars->width, farStars->height};
            SDL_RenderTexture(renderer, farStars->texture, NULL, &dst);
        }
    }
    for (float y = nearStars->yOffSet; y < windowHeight; y += nearStars->height) {
        for (float x = 0; x < windowWidth; x += nearStars->width) {
            SDL_FRect dst = {x, y, nearStars->width, nearStars->height};
            SDL_RenderTexture(renderer, nearStars->texture, NULL, &dst);
        }
    }
}
