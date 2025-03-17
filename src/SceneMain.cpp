#include "SceneMain.h"

#include <iostream>
#include <string>

#include "Game.h"
#include "SceneEnd.h"
#include "SceneTitle.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void SceneMain::init() {
    game.setFinalScore(0);
    
    std::random_device rd;
    gen = std::mt19937(rd());
    dis = std::uniform_real_distribution<float>(0.0f, 1.0f);

    // Load and play background music
    bgm = Mix_LoadMUS("../assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    if (bgm == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load music: %s", SDL_GetError());
        return;
    }
    if (!Mix_PlayMusic(bgm, -1)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to play music: %s", SDL_GetError());
        return;
    }

    // Load sound effects
    soundEffects["playerShoot"] = Mix_LoadWAV("../assets/sound/laser_shoot4.wav");
    if (soundEffects["playerShoot"] == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load sound effect: %s", SDL_GetError());
        return;
    }
    soundEffects["playerExplosion"] = Mix_LoadWAV("../assets/sound/explosion1.wav");
    if (soundEffects["playerExplosion"] == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load sound effect: %s", SDL_GetError());
        return;
    }
    soundEffects["enemyShoot"] = Mix_LoadWAV("../assets/sound/xs_laser.wav");
    if (soundEffects["enemyShoot"] == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load sound effect: %s", SDL_GetError());
        return;
    }
    soundEffects["enemyExplosion"] = Mix_LoadWAV("../assets/sound/explosion3.wav");
    if (soundEffects["enemyExplosion"] == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load sound effect: %s", SDL_GetError());
        return;
    }
    soundEffects["itemGet"] = Mix_LoadWAV("../assets/sound/eff5.wav");
    if (soundEffects["itemGet"] == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load sound effect: %s", SDL_GetError());
        return;
    }
    soundEffects["hit"] = Mix_LoadWAV("../assets/sound/eff11.wav");
    if (soundEffects["hit"] == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load sound effect: %s", SDL_GetError());
        return;
    }

    scoreFont = TTF_OpenFont("../assets/font/VonwaonBitmap-12px.ttf", 24);

    player.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/SpaceShip.png");
    if (!player.texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load texture: %s", SDL_GetError());
        return;
    }
    SDL_GetTextureSize(player.texture, &player.width, &player.height);
    player.width /= 5;
    player.height /= 5;
    player.position.x = game.getWindowWidth() / 2 - player.width / 2;
    player.position.y = game.getWindowHeight() - player.height;

    healthIcon = IMG_LoadTexture(game.getRenderer(), "../assets/image/Health UI Black.png");

    shield.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/shield.png");
    if (!shield.texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load shield texture: %s", SDL_GetError());
        return;
    }
    SDL_GetTextureSize(shield.texture, &shield.width, &shield.height);
    shield.width /= 4;
    shield.height /= 4;

    EnemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/insect-1.png");
    if (!EnemyTemplate.texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load EnemyTemplate texture: %s", SDL_GetError());
    }
    SDL_GetTextureSize(EnemyTemplate.texture, &EnemyTemplate.width, &EnemyTemplate.height);
    EnemyTemplate.width /= 4;
    EnemyTemplate.height /= 4;

    BulletPlayerTemplate.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/bullet.png");
    if (!BulletPlayerTemplate.texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load BulletPlayerTemplate texture: %s", SDL_GetError());
    }
    SDL_GetTextureSize(BulletPlayerTemplate.texture, &BulletPlayerTemplate.width, &BulletPlayerTemplate.height);
    BulletPlayerTemplate.width /= 3;
    BulletPlayerTemplate.height /= 3;

    BulletEnemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/laser-1.png");
    if (!BulletEnemyTemplate.texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load BulletEnemyTemplate texture: %s", SDL_GetError());
    }
    SDL_GetTextureSize(BulletEnemyTemplate.texture, &BulletEnemyTemplate.width, &BulletEnemyTemplate.height);
    BulletEnemyTemplate.width /= 4;
    BulletEnemyTemplate.height /= 4;

    explosionTemplate.texture = IMG_LoadTexture(game.getRenderer(), "../assets/effect/explosion.png");
    if (!explosionTemplate.texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load explosionTemplate texture: %s", SDL_GetError());
    }
    SDL_GetTextureSize(explosionTemplate.texture, &explosionTemplate.width, &explosionTemplate.height);
    explosionTemplate.totalFrame = static_cast<int>(explosionTemplate.width / explosionTemplate.height);
    explosionTemplate.height *= 2;
    explosionTemplate.width = explosionTemplate.height;

    itemHealthTemplate.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/bonus_life.png");
    if (!itemHealthTemplate.texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load itemHealthTemplate texture: %s", SDL_GetError());
    }
    itemHealthTemplate.type = ItemType::Health;
    SDL_GetTextureSize(itemHealthTemplate.texture, &itemHealthTemplate.width, &itemHealthTemplate.height);
    itemHealthTemplate.width /= 4;
    itemHealthTemplate.height /= 4;

    itemShieldTemplate.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/bonus_shield.png");
    if (!itemShieldTemplate.texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load itemShieldTemplate texture: %s", SDL_GetError());
    }
    itemShieldTemplate.type = ItemType::Shield;
    SDL_GetTextureSize(itemShieldTemplate.texture, &itemShieldTemplate.width, &itemShieldTemplate.height);
    itemShieldTemplate.width /= 4;
    itemShieldTemplate.height /= 4;

    itemTimeTemplate.texture = IMG_LoadTexture(game.getRenderer(), "../assets/image/bonus_time.png");
    if (!itemTimeTemplate.texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load itemTimeTemplate texture: %s", SDL_GetError());
    }
    itemTimeTemplate.type = ItemType::Time;
    SDL_GetTextureSize(itemTimeTemplate.texture, &itemTimeTemplate.width, &itemTimeTemplate.height);
    itemTimeTemplate.width /= 4;
    itemTimeTemplate.height /= 4;
}

void SceneMain::update(float deltaTime) {
    playerMovement(deltaTime);
    shieldUpdate(deltaTime);
    bulletsPlayerUpdate(deltaTime);
    bulletsEnemyUpdate(deltaTime);
    enemyCreate();
    enemyUpdate(deltaTime);
    playerUpdate(deltaTime);
    explosionsUpdate(deltaTime);
    itemCreate();
    itemUpdate(deltaTime);
    if (!isPlayerAlive) {
        changeSceneDelayed(deltaTime, 2.0f);
    }
}

void SceneMain::render() {
    for (auto bullet : bulletsPlayer) {
        SDL_FRect bulletRect = {bullet->position.x, bullet->position.y, bullet->width, bullet->height};
        SDL_RenderTexture(game.getRenderer(), bullet->texture, NULL, &bulletRect);
    }

    if (isPlayerAlive) {
        SDL_FRect playerRect = {player.position.x, player.position.y, player.width, player.height};
        SDL_RenderTexture(game.getRenderer(), player.texture, NULL, &playerRect);
        if (player.shield) {
            SDL_FRect shieldRect = {shield.position.x, shield.position.y, shield.width, shield.height};
            SDL_RenderTexture(game.getRenderer(), shield.texture, NULL, &shieldRect);
        }
    }

    for (auto bullet : bulletsEnemy) {
        SDL_FRect bulletRect = {bullet->position.x, bullet->position.y, bullet->width, bullet->height};
        auto angle = atan2(bullet->direction.y, bullet->direction.x) * 180 / M_PI - 90;
        SDL_RenderTextureRotated(game.getRenderer(), bullet->texture, NULL, &bulletRect, angle, NULL, SDL_FLIP_NONE);
    }

    for (auto item : items) {
        SDL_FRect itemRect = {item->position.x, item->position.y, item->width, item->height};
        SDL_RenderTexture(game.getRenderer(), item->texture, NULL, &itemRect);
    }

    for (auto enemy : enemies) {
        SDL_FRect enemyRect = {enemy->position.x, enemy->position.y, enemy->width, enemy->height};
        SDL_RenderTexture(game.getRenderer(), enemy->texture, NULL, &enemyRect);
    }

    for (auto explosion : explosions) {
        SDL_FRect srcRect = {explosion->currentFrame * explosion->height, 0, explosion->height / 2, explosion->height / 2};
        SDL_FRect explosionRect = {explosion->position.x, explosion->position.y, explosion->width, explosion->height};
        SDL_RenderTexture(game.getRenderer(), explosion->texture, &srcRect, &explosionRect);
    }

    UIRender();
}

void SceneMain::clean() {
    for (auto soundEffect : soundEffects) {
        if (soundEffect.second) {
            Mix_FreeChunk(soundEffect.second);
            soundEffect.second = nullptr;
        }
    }
    soundEffects.clear();
    
    for (auto bullet : bulletsPlayer) {
        if (bullet)
        delete bullet;
    }
    bulletsPlayer.clear();
    
    for (auto bullet : bulletsEnemy) {
        if (bullet)
        delete bullet;
    }
    bulletsEnemy.clear();
    
    for (auto enemy : enemies) {
        if (enemy)
        delete enemy;
    }
    enemies.clear();
    
    for (auto explosion : explosions) {
        if (explosion)
        delete explosion;
    }
    explosions.clear();
    
    for (auto item : items) {
        if (item)
            delete item;
    }
    items.clear();

    if(bgm) {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
        bgm = nullptr;
    }

    if (scoreFont) {
        TTF_CloseFont(scoreFont);
        scoreFont = nullptr;
    }

    if (player.texture) {
        SDL_DestroyTexture(player.texture);
        player.texture = nullptr;
    }
    if (healthIcon) {
        SDL_DestroyTexture(healthIcon);
        healthIcon = nullptr;
    }
    if (shield.texture) {
        SDL_DestroyTexture(shield.texture);
        shield.texture = nullptr;
    }
    if (BulletPlayerTemplate.texture) {
        SDL_DestroyTexture(BulletPlayerTemplate.texture);
        BulletPlayerTemplate.texture = nullptr;
    }
    if (BulletEnemyTemplate.texture) {
        SDL_DestroyTexture(BulletEnemyTemplate.texture);
        BulletEnemyTemplate.texture = nullptr;
    }
    if (EnemyTemplate.texture) {
        SDL_DestroyTexture(EnemyTemplate.texture);
        EnemyTemplate.texture = nullptr;
    }
    if (explosionTemplate.texture) {
        SDL_DestroyTexture(explosionTemplate.texture);
        explosionTemplate.texture = nullptr;
    }
}

void SceneMain::handleEvent(SDL_Event* sdlEvent) {
    if (sdlEvent->type == SDL_EVENT_KEY_DOWN) {
        if (sdlEvent->key.scancode == SDL_SCANCODE_ESCAPE) {
            game.changeScene(new SceneTitle());
        } else if (sdlEvent->key.scancode == SDL_SCANCODE_BACKSPACE) {
            game.pause();
        } else if (sdlEvent->key.scancode == SDL_SCANCODE_KP_ENTER || sdlEvent->key.scancode == SDL_SCANCODE_RETURN) {
            game.resume();
        } else if (sdlEvent->key.scancode == SDL_SCANCODE_TAB) {
            isPlayerAlive = false;
            game.setFinalScore(player.score);
            game.changeScene(new SceneEnd());
        }
    }
}

void SceneMain::playerMovement(float deltaTime) {
    auto keyBoardState = SDL_GetKeyboardState(NULL);
    if (!isPlayerAlive) {
        return;
    }
    if (keyBoardState[SDL_SCANCODE_D] || keyBoardState[SDL_SCANCODE_RIGHT]) {
        player.position.x += player.speed * deltaTime;
    }
    if (keyBoardState[SDL_SCANCODE_A] || keyBoardState[SDL_SCANCODE_LEFT]) {
        player.position.x -= player.speed * deltaTime;
    }
    if (keyBoardState[SDL_SCANCODE_W] || keyBoardState[SDL_SCANCODE_UP]) {
        player.position.y -= player.speed * deltaTime;
    }
    if (keyBoardState[SDL_SCANCODE_S] || keyBoardState[SDL_SCANCODE_DOWN]) {
        player.position.y += player.speed * deltaTime;
    }
    player.position.x = player.position.x < 0 ? 0 : player.position.x;
    player.position.x = player.position.x > game.getWindowWidth() - player.width ? game.getWindowWidth() - player.width
                                                                                 : player.position.x;
    player.position.y = player.position.y < 0 ? 0 : player.position.y;
    player.position.y = player.position.y > game.getWindowHeight() - player.height
                            ? game.getWindowHeight() - player.height
                            : player.position.y;

    if (keyBoardState[SDL_SCANCODE_SPACE] || keyBoardState[SDL_SCANCODE_J]) {
        Uint64 currentTime = SDL_GetTicks();
        if (currentTime - player.lastShotTime > player.shotDelay) {
            player.lastShotTime = currentTime;
            playerShoot();
        }
    }
}

void SceneMain::playerShoot() {
    auto bullet = new Bullet(BulletPlayerTemplate);
    bullet->position.x = player.position.x + player.width / 2 - bullet->width / 2;
    bullet->position.y = player.position.y - bullet->height;
    bulletsPlayer.push_back(bullet);
    if (soundEffects["playerShoot"]) {
        Mix_PlayChannel(0, soundEffects["playerShoot"], 0);
    }
}

void SceneMain::playerUpdate(float) {
    if (!isPlayerAlive) {
        return;
    }
    if (player.health <= 0) {
        isPlayerAlive = false;
        // create explosion for player
        auto explosion = new Explosion(explosionTemplate);
        explosion->position.x = player.position.x + player.width / 2 - explosion->width / 2;
        explosion->position.y = player.position.y + player.height / 2 - explosion->height / 2;
        auto currentTime = SDL_GetTicks();
        explosion->startTime = currentTime;
        explosions.push_back(explosion);
        if (soundEffects["playerExplosion"]) {
            Mix_PlayChannel(-1, soundEffects["playerExplosion"], 0);
        }
        return;
    }
    // check collision with enemy
    for (auto enemy : enemies) {
        SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y),
                               static_cast<int>(player.width), static_cast<int>(player.height)};
        SDL_Rect enemyRect = {static_cast<int>(enemy->position.x), static_cast<int>(enemy->position.y),
                              static_cast<int>(enemy->width), static_cast<int>(enemy->height)};
        if (SDL_HasRectIntersection(&playerRect, &enemyRect)) {
            if (player.shield) {
                player.shield -= 1;
                enemy->health = 0;
            } else {
                player.health -= 1;
                enemy->health = 0;
            }
        }
    }
}

void SceneMain::bulletsPlayerUpdate(float deltaTime) {
    float maxBulletY = -BulletPlayerTemplate.height / 2;
    for (auto iter = bulletsPlayer.begin(); iter != bulletsPlayer.end();) {
        auto bullet = *iter;
        bullet->position.y -= bullet->speed * deltaTime;
        if (bullet->position.y < maxBulletY) {
            delete bullet;
            iter = bulletsPlayer.erase(iter);
        } else {
            // check collision
            bool hit = false;
            for (auto enemy : enemies) {
                SDL_Rect bulletRect = {static_cast<int>(bullet->position.x), static_cast<int>(bullet->position.y),
                                       static_cast<int>(bullet->width), static_cast<int>(bullet->height)};
                SDL_Rect enemyRect = {static_cast<int>(enemy->position.x), static_cast<int>(enemy->position.y),
                                      static_cast<int>(enemy->width), static_cast<int>(enemy->height)};
                if (SDL_HasRectIntersection(&bulletRect, &enemyRect)) {
                    hit = true;
                    enemy->health -= bullet->damage;
                    delete bullet;
                    if (soundEffects["hit"]) {
                        Mix_PlayChannel(-1, soundEffects["hit"], 0);
                    }
                    iter = bulletsPlayer.erase(iter);
                    break;
                }
            }
            if (!hit) {
                iter++;
            }
        }
    }
}

void SceneMain::bulletsEnemyUpdate(float deltaTime) {
    float maxBulletX = game.getWindowWidth();
    float maxBulletY = game.getWindowHeight();
    for (auto iter = bulletsEnemy.begin(); iter != bulletsEnemy.end();) {
        auto bullet = *iter;
        bullet->position.x += bullet->speed * bullet->direction.x * deltaTime;
        bullet->position.y += bullet->speed * bullet->direction.y * deltaTime;
        if (bullet->position.y > maxBulletY || bullet->position.x < 0 || bullet->position.x > maxBulletX ||
            bullet->position.y < 0) {
            delete bullet;
            iter = bulletsEnemy.erase(iter);
        } else {
            // check collision
            SDL_Rect bulletRect = {static_cast<int>(bullet->position.x), static_cast<int>(bullet->position.y),
                                   static_cast<int>(bullet->width), static_cast<int>(bullet->height)};
            SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y),
                                   static_cast<int>(player.width), static_cast<int>(player.height)};
            if (SDL_HasRectIntersection(&bulletRect, &playerRect) && isPlayerAlive) {
                if (player.shield) {
                    player.shield -= bullet->damage;
                } else {
                    player.health -= bullet->damage;
                }
                delete bullet;
                if (soundEffects["hit"]) {
                    Mix_PlayChannel(-1, soundEffects["hit"], 0);
                }
                iter = bulletsEnemy.erase(iter);
            } else
                iter++;
        }
    }
}

void SceneMain::enemyCreate() {
    if (dis(gen) > 1 / 60.0f) {
        return;
    }
    Enemy* enemy = new Enemy(EnemyTemplate);
    enemy->position.x = dis(gen) * (game.getWindowWidth() - enemy->width);
    enemy->position.y = -enemy->height;
    enemies.push_back(enemy);
}

void SceneMain::enemyUpdate(float deltaTime) {
    auto currentTime = SDL_GetTicks();
    for (auto iter = enemies.begin(); iter != enemies.end();) {
        auto enemy = *iter;
        enemy->position.y += enemy->speed * deltaTime;
        if (enemy->position.y > game.getWindowHeight()) {
            delete enemy;
            iter = enemies.erase(iter);
        } else {
            if (currentTime - enemy->lastShotTime > enemy->shotDelay && isPlayerAlive) {
                enemy->lastShotTime = currentTime;
                enemyShoot(enemy);
            }
            if (enemy->health <= 0) {
                enemyExplode(enemy);
                if (soundEffects["enemyExplosion"]) {
                    Mix_PlayChannel(-1, soundEffects["enemyExplosion"], 0);
                }
                iter = enemies.erase(iter);
            } else {
                iter++;
            }
        }
    }
}

void SceneMain::enemyExplode(Enemy* enemy) {
    auto explosion = new Explosion(explosionTemplate);
    explosion->position.x = enemy->position.x + enemy->width / 2 - explosion->width / 2;
    explosion->position.y = enemy->position.y + enemy->height / 2 - explosion->height / 2;
    auto currentTime = SDL_GetTicks();
    explosion->startTime = currentTime;
    explosions.push_back(explosion);
    player.score += 10;
    delete enemy;
}

void SceneMain::enemyShoot(Enemy* enemy) {
    auto bullet = new Bullet(BulletEnemyTemplate);
    bullet->position.x = enemy->position.x + enemy->width / 2 - bullet->width / 2;
    bullet->position.y = enemy->position.y + enemy->height / 2 - bullet->height / 2;
    bullet->direction = getDirection(bullet->position, player.position);
    bulletsEnemy.push_back(bullet);
    if (soundEffects["enemyShoot"]) {
        Mix_PlayChannel(-1, soundEffects["enemyShoot"], 0);
    }
}

SDL_FPoint SceneMain::getDirection(SDL_FPoint from, SDL_FPoint to) {
    auto x = to.x - from.x;
    auto y = to.y - from.y;
    auto length = sqrt(x * x + y * y);
    return length > 0 ? SDL_FPoint{x / length, y / length} : SDL_FPoint{0, 1};
}

void SceneMain::explosionsUpdate(float) {
    auto currentTime = SDL_GetTicks();
    for (auto iter = explosions.begin(); iter != explosions.end();) {
        auto explosion = *iter;
        explosion->currentFrame = static_cast<int>((currentTime - explosion->startTime) * explosion->FPS / 1000);
        if (explosion->currentFrame >= explosion->totalFrame) {
            delete explosion;
            iter = explosions.erase(iter);
        } else {
            iter++;
        }
    }
}

void SceneMain::itemCreate() {
    if (dis(gen) > 0.01f) {
        return;
    }
    auto itemType = static_cast<ItemType>(rand() % 3);
    Item* item = nullptr;
    switch (itemType) {
        case ItemType::Health:
            item = new Item(itemHealthTemplate);
            break;
        case ItemType::Shield:
            item = new Item(itemShieldTemplate);
            break;
        case ItemType::Time:
            item = new Item(itemTimeTemplate);
            break;
    }
    item->position.x = dis(gen) * (game.getWindowWidth() - item->width);
    item->position.y = -item->height;
    double angle = dis(gen) * M_PI;
    item->direction = {static_cast<float>(cos(angle)), static_cast<float>(sin(angle))};
    items.push_back(item);
}

void SceneMain::itemUpdate(float deltaTime) {
    for (auto iter = items.begin(); iter != items.end();) {
        auto item = *iter;
        item->position.y += item->direction.y * item->speed * deltaTime;
        item->position.x += item->direction.x * item->speed * deltaTime;
        // rim bounce
        if (item->bounceCount > 0) {
            if (item->position.x < 0 || item->position.x > game.getWindowWidth() - item->width) {
                item->direction.x = -item->direction.x;
                item->bounceCount--;
                // make sure item will be constrained in the bound
                item->position.x = std::max(0.0f, std::min(item->position.x, game.getWindowWidth() - item->width));
            }
            if (item->position.y < 0 || item->position.y > game.getWindowHeight() - item->height) {
                item->direction.y = -item->direction.y;
                item->bounceCount--;
                // make sure item will be constrained in the bound
                item->position.y = std::max(0.0f, std::min(item->position.y, game.getWindowHeight() - item->height));
            }
        }
        // if the item is out of screen, delete it
        if ((item->position.y > game.getWindowHeight() || item->position.x + item->width < 0 ||
             item->position.x > game.getWindowWidth() || item->position.y + item->height < 0)) {
            delete item;
            iter = items.erase(iter);
        } else {
            SDL_Rect itemRect = {static_cast<int>(item->position.x), static_cast<int>(item->position.y),
                                 static_cast<int>(item->width), static_cast<int>(item->height)};
            SDL_Rect playerRect = {static_cast<int>(player.position.x), static_cast<int>(player.position.y),
                                   static_cast<int>(player.width), static_cast<int>(player.height)};
            if (SDL_HasRectIntersection(&itemRect, &playerRect)) {
                player.score += 5;
                if (soundEffects["itemGet"]) {
                    Mix_PlayChannel(-1, soundEffects["itemGet"], 0);
                }
                // apply item effect
                switch (item->type) {
                    case ItemType::Health:
                        player.health = player.health < player.maxHealth ? player.health + 1 : player.health;
                        break;
                    case ItemType::Shield:
                        player.shield = player.shield < 1 ? player.shield + 1 : player.shield;
                        break;
                    case ItemType::Time:
                        player.shotDelay = player.shotDelay - 100 > 100 ? player.shotDelay - 100 : player.shotDelay;
                        break;
                }
                delete item;
                iter = items.erase(iter);
            } else {
                iter++;
            }
        }
    }
}

void SceneMain::shieldUpdate(float) {
    shield.position.x = player.position.x + player.width / 2 - shield.width / 2;
    shield.position.y = player.position.y - shield.height / 2;
}

void SceneMain::UIRender() {
    // render health
    float x = 10.0f, y = 10.0f;
    float size = 32.0f;
    int offset = 40;
    SDL_SetTextureColorMod(healthIcon, 100, 100, 100);  // dark color
    for (float i = 0; i < player.maxHealth; i++) {
        SDL_FRect rect = {x + i * offset, y, size, size};
        SDL_RenderTexture(game.getRenderer(), healthIcon, NULL, &rect);
    }
    SDL_SetTextureColorMod(healthIcon, 255, 255, 255);  // reset color
    for (float i = 0; i < player.health; i++) {
        SDL_FRect rect = {x + i * offset, y, size, size};
        SDL_RenderTexture(game.getRenderer(), healthIcon, NULL, &rect);
    }
    // render score
    auto text = "SCORE: " + std::to_string(player.score);
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(scoreFont, text.c_str(), 0, textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(game.getRenderer(), surface);
    SDL_FRect textRect = {game.getWindowWidth() - surface->w - 10, 10, static_cast<float>(surface->w),
                          static_cast<float>(surface->h)};
    SDL_RenderTexture(game.getRenderer(), texture, NULL, &textRect);
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
}

void SceneMain::changeSceneDelayed(float deltaTime, float delay) {
    timerEnd += deltaTime;
    if (timerEnd > delay) {
        game.setFinalScore(player.score);
        game.changeScene(new SceneEnd());
    }
}
