#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <SDL3/SDL.h>

struct Background{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    float width = 0;
    float height = 0;
    float speed = 30;
    float yOffSet = 0;
};

struct Player{
    int id;
    int score = 0;
    int health = 5;
    int maxHealth = 5;
    int shield = 0;

    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    int speed = 200;
    float width = 0;
    float height = 0;

    Uint64 lastShotTime = 0;
    Uint32 shotDelay = 200;
    Uint64 aliveTime = 0;
};

struct Enemy{
    int id;
    int health = 2;
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    int speed = 100;
    float width = 0;
    float height = 0;
    Uint64 lastShotTime = 0;
    Uint32 shotDelay = 1500;
};

struct Bullet{
    int id;
    int damage = 1;
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    SDL_FPoint direction = {0, 0};
    int speed = 400;
    float width = 0;
    float height = 0;
};

struct Explosion{
    int id;
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    float width = 0;
    float height = 0;
    int currentFrame = 0;
    int totalFrame = 0;
    Uint64 startTime = 0;
    Uint32 FPS = 10;
};

enum class ItemType{
    Health,
    Shield,
    Time
};

struct Item{
    int id;
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    SDL_FPoint direction = {0, 1};
    int bounceCount = 3;
    int speed = 300;
    float width = 0;
    float height = 0;
    ItemType type = ItemType::Health;
};

struct Shield{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0, 0};
    float width = 0;
    float height = 0;
};

#endif