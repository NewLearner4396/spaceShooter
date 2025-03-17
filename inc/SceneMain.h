#ifndef __SceneMain_H__
#define __SceneMain_H__

#include <list>
#include <map>
#include <random>

#include "Object.h"
#include "Scene.h"

class SceneMain : public Scene {
   public:
    // SceneMain();
    // ~SceneMain();

    void init() override;
    void update(float deltaTime) override;
    void render() override;
    void clean() override;
    void handleEvent(SDL_Event* sdlEvent) override;

   private:
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;

    float timerEnd = 0;

    TTF_Font* scoreFont = nullptr;
    
    Player player;
    bool isPlayerAlive = true;
    Shield shield;
    SDL_Texture* healthIcon;
    
    Enemy EnemyTemplate;
    std::list<Enemy*> enemies;
    
    Bullet BulletPlayerTemplate;
    std::list<Bullet*> bulletsPlayer;
    
    Bullet BulletEnemyTemplate;
    std::list<Bullet*> bulletsEnemy;
    
    Explosion explosionTemplate;
    std::list<Explosion*> explosions;

    std::list<Item*> items;
    Item itemHealthTemplate;
    Item itemShieldTemplate;
    Item itemTimeTemplate;

    Mix_Music* bgm = nullptr;
    std::map<std::string, Mix_Chunk*> soundEffects;

    void playerMovement(float deltaTime);
    void playerShoot();
    void playerUpdate(float deltaTime);

    void bulletsPlayerUpdate(float deltaTime);
    void bulletsEnemyUpdate(float deltaTime);

    void enemyCreate();
    void enemyUpdate(float deltaTime);
    void enemyExplode(Enemy* enemy);
    void enemyShoot(Enemy* enemy);
    SDL_FPoint getDirection(SDL_FPoint from, SDL_FPoint to);
    void explosionsUpdate(float deltaTime);

    void itemCreate();
    void itemUpdate(float deltaTime);
    void shieldUpdate(float deltaTime);

    void UIRender();
    void changeSceneDelayed(float deltaTime, float delay);

};

#endif  // __SceneMain_H__
