#ifndef SCENETITLE_H
#define SCENETITLE_H

#include "Scene.h"

class SceneTitle : public Scene {
public:

    void init() override;
    void clean() override;
    void update(float dT) override;
    void render() override;
    void handleEvent(SDL_Event* sdlEvent) override;

private:
    float timer = 0;
    Mix_Music* bgm = nullptr;
};

#endif