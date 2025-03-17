#ifndef __SceneEnd_H__
#define __SceneEnd_H__

#include "Scene.h"

class SceneEnd : public Scene {
   public:
    void init() override;
    void clean() override;
    void update(float deltaTime) override;
    void render() override;
    void handleEvent(SDL_Event* event) override;

   private:
    Mix_Music* bgm = nullptr;
    bool isTyping = true;
    std::string playerName = "";
    float cursorBlinkTimer = 0;
    float cursorBlinkDelay = 0.5f;
    float textBlinkTimer = 0;
    float textBlinkDelay = 0.75f;

    void removeLastUTF8Char(std::string& str);
};

#endif  // __SceneEnd_H__
