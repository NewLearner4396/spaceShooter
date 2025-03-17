#ifndef __SCENE_H__
#define __SCENE_H__

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

class Game; // Re-adding the Game class forward declaration

class Scene {
public:
    Scene();
    virtual ~Scene() = default;

    virtual void init() = 0;
    virtual void clean() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void handleEvent(SDL_Event* sdlEvent) = 0;
    
private:

protected:
    Game& game;
};

#endif // __SCENE_H__