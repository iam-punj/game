#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>

struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };

class Game
{
    sf::RenderWindow g_window;
    EntityManager g_entitymanager;
    sf::Font g_font;
    sf::Text g_text;
    sf::Text g_text_1;
    sf::Text g_text_2;
    sf::Music shoot;
    sf::Music dead; 
    sf::Music bg1; 
    sf::Music special; 
    PlayerConfig g_playerconfig;
    EnemyConfig g_enemyconfig;
    BulletConfig g_bulletconfig;
    int g_score = 0;
    int g_current_frame = 0;
    int g_lastEnemySpawnTime = 0;
    bool g_pause = 0;
    bool g_running = 1;

    Entity* g_player;

    void init(const std::string& config);
    void setPaused(bool& frame_lock, int& frame_lock_f);
    void sMovement();
    void sUserInput(bool& special_weapon, int& frame_lock_2);
    void sLifespan();
    void sRender();
    void sEnemySpawner();
    void sCollision();

    int rand_num(int min, int max);
    void spawnplayer();
    void spawnEnemy();
    void spawnSmallEnemies(Entity* entity);
    void spawnBullets(Entity* entity, Vec2 mousePos);
    void spawnSpecialWeapon(Entity* entity);

public:

    Game(const std::string& config);
    void run();
};