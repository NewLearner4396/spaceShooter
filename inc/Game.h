#ifndef __GAME_H__
#define __GAME_H__
#include <map>
#include <string>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "Object.h"
#include "Scene.h"

enum class textType {
    Title,
    Text,
};

class Game {
   public:
    // Singleton pattern
    static Game& getInstance() {
        static Game instance;
        return instance;
    }
    ~Game();

    void init();
    void clean();
    void run();
    void pause();
    void resume();
    void stop();

    void changeScene(Scene* scene);

    void handleEvent(SDL_Event* event);
    void update(float deltaTime);
    void render();

    // utility functions
    void renderTextureCentered(std::string text, float posY, textType type);
    void renderTexturePos(std::string text, SDL_FPoint pos, textType type=textType::Text, bool isLeft=true);
    void saveRankBoard(std::string fileName, std::multimap<int, std::string, std::greater<int>>& rankBoard);
    void loadRankBoard(std::string fileName);

    // Getters
    SDL_Renderer* getRenderer() const { return renderer; }
    SDL_Window* getWindow() const { return window; }
    float getWindowWidth() const { return windowWidth; }
    float getWindowHeight() const { return windowHeight; }
    int getFinalScore() const { return finalScore; }
    std::multimap<int, std::string, std::greater<int>>& getRankBoard() { return rankBoard; }

    // Setters
    void setFinalScore(int score) { finalScore = score; }
    void insertRankBoard(int score, std::string name) {
        rankBoard.insert(std::make_pair(score, name));
    }

    std::string rankBoardFileName = "../assets/data/rankBoard.dat";

   private:
    Game(); // private constructor for singleton
    // delete copy constructor and assignment operator
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    bool exeRunning = true;
    bool isRunning = false;
    int finalScore = 0;
    std::multimap<int, std::string, std::greater<int>> rankBoard;
    int rankBoardMaxSize = 1000;
    bool isFullScreen = false;
    Scene* currentScene = nullptr;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Event event;
    float windowWidth = 600;
    float windowHeight = 800;
    int frameRate = 60;
    Uint32 frameTime = 1000 / frameRate;
    float deltaTime = 0;

    Background* nearStars;
    Background* farStars;
    void backgroundUpdate(float deltaTime);
    void backgroundRender();

    TTF_Font* titleFont = nullptr;
    TTF_Font* textFont = nullptr;
};

#endif  // __GAME_H__