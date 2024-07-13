#include "Game.h"
#include <fstream>
#include <iostream>
#include <random>

using namespace std;

long long g_w; 
long long g_h; 



Game::Game(const std::string& config) {
    init(config);
}

void Game::init(const std::string& config) {
    std::ifstream a("./../bin/config.txt");
    std::string str;
    a >> str;
    //window
    int w, h, framerate, fullscreen;
    a >> w; a >> h; a >> framerate; a >> fullscreen;
    g_w = w;
    g_h = h; 

    if (fullscreen == 1)
        g_window.create(sf::VideoMode(w, h), "Assignment2", sf::Style::Fullscreen);
    else g_window.create(sf::VideoMode(w, h), "Assignment2");
    g_window.setFramerateLimit(framerate);

    //font
    a >> str;
    string path; int char_size, font_r, font_g, font_b;
    a >> path; a >> char_size; a >> font_r; a >> font_g; a >> font_b;

    g_font.loadFromFile(path);
    g_text.setFont(g_font);
    g_text.setCharacterSize(char_size);
    g_text.setFillColor(sf::Color(font_r, font_g, font_b));
    //added_text
    g_text_1.setFont(g_font);
    g_text_1.setCharacterSize(char_size / 2);
    g_text_1.setFillColor(sf::Color(50, 50, 50));
    g_text_2.setFont(g_font);
    g_text_2.setCharacterSize(char_size);
    g_text_2.setFillColor(sf::Color(150, 150, 150));
    g_text_2.setString("YOU HAVE SPECIAL ! ");

    //player
    a >> str;
    a >> g_playerconfig.SR >> g_playerconfig.CR >> g_playerconfig.FR >> g_playerconfig.FG >> g_playerconfig.FB >> g_playerconfig.OR >> g_playerconfig.OG >> g_playerconfig.OB >> g_playerconfig.OT >> g_playerconfig.V >> g_playerconfig.S;

    //enemy 
    a >> str;
    a >> g_enemyconfig.SR >> g_enemyconfig.CR >> g_enemyconfig.OR >> g_enemyconfig.OG >> g_enemyconfig.OB >> g_enemyconfig.OT >> g_enemyconfig.VMIN >> g_enemyconfig.VMAX >> g_enemyconfig.L >> g_enemyconfig.SI >> g_enemyconfig.SMIN >> g_enemyconfig.SMAX;

    //bullet
    a >> str;
    a >> g_bulletconfig.SR >> g_bulletconfig.CR >> g_bulletconfig.FR >> g_bulletconfig.FG >> g_bulletconfig.FB >> g_bulletconfig.OR >> g_bulletconfig.OG >> g_bulletconfig.OB >> g_bulletconfig.OT >> g_bulletconfig.V >> g_bulletconfig.L >> g_bulletconfig.S;
}

void Game::run() {

    //some variables 
    bool frame_lock = 0;
    int frame_lock_f = 0;
    int frame_lock_2 = 0;
    bool special_weapon = 0;

    spawnplayer();

    // additional for scene 1 ////////////////////////////////////////////////////////////////
    sf::Music m1;
    
    if ( m1.openFromFile("./../audio/Bg1.mp3")) {
        m1.play();
    }

    bool s1 = 1; 
    sf::Text t1; 

    t1.setFont(g_font); 
    t1.setString("Shape wars");
    t1.setCharacterSize(60);
    t1.setFillColor(sf::Color::Cyan);
    t1.setStyle(sf::Text::Bold );
    t1.setPosition( g_w/2 - t1.getLocalBounds().width/2 , g_h/2 -t1.getLocalBounds().height/2 - 50);



    sf::Text t2;

    t2.setFont(g_font);
    t2.setString("Press any key to continue !!");
    t2.setCharacterSize(20);
    t2.setFillColor(sf::Color::White);
    t2.setPosition(g_w / 2 - t2.getLocalBounds().width / 2 , g_h / 2 - t2.getLocalBounds().height / 2 + 50 );

    sf::Text t3;
    t3.setString("");

    while (g_window.isOpen() && s1 ) {
        sf::Event event;
        while (g_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { g_window.close(); }
        }

        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))) {
            s1 = 0; 
        }

        g_window.clear(sf::Color::Black);
        g_window.draw(t1);
        g_window.draw(t2);
        g_window.display();
    }

    m1.stop(); 


    // additional for scene 1 //////////////////////////////////////////////////////////////// over 


    if (dead.openFromFile("./../audio/dead.mp3")) {
    }
    if (bg1.openFromFile("./../audio/bg2.mp3")) {
        bg1.play();
        bg1.setVolume(35);
    }
    if (shoot.openFromFile("./../audio/Bullets.mp3")) {
    }
    if ( special.openFromFile("./../audio/special.wav"))

    while (g_window.isOpen()) {
        sf::Event event;
        while (g_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { g_window.close(); }
        }
        setPaused(frame_lock, frame_lock_f);
        if (g_running) {
            g_entitymanager.update();

            sEnemySpawner();
            sMovement();
            sCollision();
            sUserInput(special_weapon, frame_lock_2);
            sLifespan();
            sRender();
        }
        g_current_frame++;
    }
}

void Game::setPaused(bool& frame_lock, int& frame_lock_f) {
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::P) && (!frame_lock))) {
        frame_lock = true;
        g_running = !g_running;
        frame_lock_f = g_current_frame;

    }
    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::P) && (g_current_frame >= frame_lock_f + 5)) { frame_lock = false; frame_lock_f = 0; }

}

void Game::sMovement() {



    for (auto x : g_entitymanager.getentity()) {
        if (x->tag() != "player") {
            x->cTransform->pos += x->cTransform->vel;
        }
        x->cShape->Circle.setPosition(x->cTransform->pos.x, x->cTransform->pos.y);
        x->cShape->Circle.setRotation(x->cShape->Circle.getRotation() + 3);
    }

    for (auto x : g_entitymanager.getentity("enemy")) {
        if (x->cTransform->pos.x < x->cShape->Circle.getRadius() || x->cTransform->pos.x > g_window.getSize().x - x->cShape->Circle.getRadius()) { x->cTransform->vel.x = -x->cTransform->vel.x; }
        if (x->cTransform->pos.y < x->cShape->Circle.getRadius() || x->cTransform->pos.y > g_window.getSize().y - x->cShape->Circle.getRadius()) { x->cTransform->vel.y = -x->cTransform->vel.y; }
    }
    for (auto x : g_entitymanager.getentity("player")) {
        if (x->cTransform->pos.x < x->cShape->Circle.getRadius()) { x->cTransform->pos.x = x->cShape->Circle.getRadius(); }
        if (x->cTransform->pos.y < x->cShape->Circle.getRadius()) { x->cTransform->pos.y = x->cShape->Circle.getRadius(); }
        if (x->cTransform->pos.y > g_window.getSize().y - x->cShape->Circle.getRadius()) { x->cTransform->pos.y = g_window.getSize().y - x->cShape->Circle.getRadius(); }
        if (x->cTransform->pos.x > g_window.getSize().x - x->cShape->Circle.getRadius()) { x->cTransform->pos.x = g_window.getSize().x - x->cShape->Circle.getRadius(); }
    }

}

void Game::sUserInput(bool& special_weapon, int& frame_lock_2) {

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        g_player->cTransform->pos.x -= g_player->cTransform->vel.x;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        g_player->cTransform->pos.x += g_player->cTransform->vel.x;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        g_player->cTransform->pos.y -= g_player->cTransform->vel.y;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        g_player->cTransform->pos.y += g_player->cTransform->vel.y;
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && g_current_frame % 10 == 0) {
        if (!special_weapon) {
            shoot.play();
            spawnBullets(g_player, Vec2(sf::Mouse::getPosition(g_window).x, sf::Mouse::getPosition(g_window).y));
        }
        else if (special_weapon) spawnSpecialWeapon(g_player);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && frame_lock_2 == 0) {

        frame_lock_2 = g_current_frame;
        //run 
        special_weapon = !special_weapon;
       
        g_text_2.setString("SPECIAL ACTIVATED !! ");
        g_text_2.setFillColor(sf::Color(150, 0, 0));
    }
    else if (frame_lock_2 + 300 == g_current_frame && !sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && special_weapon == 1) { special_weapon = !special_weapon;  g_text_2.setString("SPECIAL USED...WAIT !! "); g_text_2.setFillColor(sf::Color(0, 0, 100)); }
    else if (frame_lock_2 + 1200 == g_current_frame && !sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        frame_lock_2 = 0;
        g_text_2.setString("SPECIAL IS AVAILABLE !! ");
        g_text_2.setFillColor(sf::Color(150, 150, 150));

    }

}

void Game::sLifespan() {
    for (auto x : g_entitymanager.getentity("bullet")) {
        x->cLifespan->remaining = x->cLifespan->remaining - 25;
        if (x->cLifespan->remaining % 5 == 0) {
            if (x->cShape->Circle.getOutlineColor().r - 7 > 0) {
                x->cShape->Circle.setOutlineColor(sf::Color(x->cShape->Circle.getOutlineColor().r - 7, x->cShape->Circle.getOutlineColor().g, x->cShape->Circle.getOutlineColor().b));
            }
            else x->cShape->Circle.setOutlineColor(sf::Color(0, x->cShape->Circle.getOutlineColor().g, x->cShape->Circle.getOutlineColor().b));
            if (x->cShape->Circle.getOutlineColor().g - 7 > 0) {
                x->cShape->Circle.setOutlineColor(sf::Color(x->cShape->Circle.getOutlineColor().r, x->cShape->Circle.getOutlineColor().g - 7, x->cShape->Circle.getOutlineColor().b));
            }
            else x->cShape->Circle.setOutlineColor(sf::Color(x->cShape->Circle.getOutlineColor().r, 0, x->cShape->Circle.getOutlineColor().b));
            if (x->cShape->Circle.getOutlineColor().b - 7 > 0) {
                x->cShape->Circle.setOutlineColor(sf::Color(x->cShape->Circle.getOutlineColor().r, x->cShape->Circle.getOutlineColor().g, x->cShape->Circle.getOutlineColor().b - 7));
            }
            else x->cShape->Circle.setOutlineColor(sf::Color(x->cShape->Circle.getOutlineColor().r, x->cShape->Circle.getOutlineColor().g, 0));

            if (x->cShape->Circle.getFillColor().r - 7 > 0) {
                x->cShape->Circle.setFillColor(sf::Color(x->cShape->Circle.getFillColor().r - 7, x->cShape->Circle.getFillColor().g, x->cShape->Circle.getFillColor().b));
            }
            else x->cShape->Circle.setFillColor(sf::Color(0, x->cShape->Circle.getFillColor().g, x->cShape->Circle.getFillColor().b));
            if (x->cShape->Circle.getFillColor().g - 7 > 0) {
                x->cShape->Circle.setFillColor(sf::Color(x->cShape->Circle.getFillColor().r, x->cShape->Circle.getFillColor().g - 7, x->cShape->Circle.getFillColor().b));
            }
            else x->cShape->Circle.setFillColor(sf::Color(x->cShape->Circle.getFillColor().r, 0, x->cShape->Circle.getFillColor().b));
            if (x->cShape->Circle.getFillColor().b - 7 > 0) {
                x->cShape->Circle.setFillColor(sf::Color(x->cShape->Circle.getFillColor().r, x->cShape->Circle.getFillColor().g, x->cShape->Circle.getFillColor().b - 7));
            }
            else x->cShape->Circle.setFillColor(sf::Color(x->cShape->Circle.getFillColor().r, x->cShape->Circle.getFillColor().g, 0));

        }
        if (x->cLifespan->remaining <= 0) {
            g_entitymanager.remove(x);
        }
    }
    for (auto x : g_entitymanager.getentity("special_bullets")) {
        x->cLifespan->remaining = x->cLifespan->remaining - 25;
        if (x->cLifespan->remaining % 5 == 0) {
            if (x->cShape->Circle.getOutlineColor().r - 7 > 0) {
                x->cShape->Circle.setOutlineColor(sf::Color(x->cShape->Circle.getOutlineColor().r - 7, x->cShape->Circle.getOutlineColor().g, x->cShape->Circle.getOutlineColor().b));
            }
            else x->cShape->Circle.setOutlineColor(sf::Color(0, x->cShape->Circle.getOutlineColor().g, x->cShape->Circle.getOutlineColor().b));
            if (x->cShape->Circle.getOutlineColor().g - 7 > 0) {
                x->cShape->Circle.setOutlineColor(sf::Color(x->cShape->Circle.getOutlineColor().r, x->cShape->Circle.getOutlineColor().g - 7, x->cShape->Circle.getOutlineColor().b));
            }
            else x->cShape->Circle.setOutlineColor(sf::Color(x->cShape->Circle.getOutlineColor().r, 0, x->cShape->Circle.getOutlineColor().b));
            if (x->cShape->Circle.getOutlineColor().b - 7 > 0) {
                x->cShape->Circle.setOutlineColor(sf::Color(x->cShape->Circle.getOutlineColor().r, x->cShape->Circle.getOutlineColor().g, x->cShape->Circle.getOutlineColor().b - 7));
            }
            else x->cShape->Circle.setOutlineColor(sf::Color(x->cShape->Circle.getOutlineColor().r, x->cShape->Circle.getOutlineColor().g, 0));

            if (x->cShape->Circle.getFillColor().r - 7 > 0) {
                x->cShape->Circle.setFillColor(sf::Color(x->cShape->Circle.getFillColor().r - 7, x->cShape->Circle.getFillColor().g, x->cShape->Circle.getFillColor().b));
            }
            else x->cShape->Circle.setFillColor(sf::Color(0, x->cShape->Circle.getFillColor().g, x->cShape->Circle.getFillColor().b));
            if (x->cShape->Circle.getFillColor().g - 7 > 0) {
                x->cShape->Circle.setFillColor(sf::Color(x->cShape->Circle.getFillColor().r, x->cShape->Circle.getFillColor().g - 7, x->cShape->Circle.getFillColor().b));
            }
            else x->cShape->Circle.setFillColor(sf::Color(x->cShape->Circle.getFillColor().r, 0, x->cShape->Circle.getFillColor().b));
            if (x->cShape->Circle.getFillColor().b - 7 > 0) {
                x->cShape->Circle.setFillColor(sf::Color(x->cShape->Circle.getFillColor().r, x->cShape->Circle.getFillColor().g, x->cShape->Circle.getFillColor().b - 7));
            }
            else x->cShape->Circle.setFillColor(sf::Color(x->cShape->Circle.getFillColor().r, x->cShape->Circle.getFillColor().g, 0));

        }
        if (x->cLifespan->remaining <= 0) {
            g_entitymanager.remove(x);
        }
    }
    for (auto x : g_entitymanager.getentity("smallenemy")) {

        x->cLifespan->remaining = x->cLifespan->remaining - 20;
        if (x->cLifespan->remaining % 5 == 0) {
            if (x->cShape->Circle.getOutlineColor().r - 7 > 0) {
                x->cShape->Circle.setOutlineColor(sf::Color(x->cShape->Circle.getOutlineColor().r - 7, x->cShape->Circle.getOutlineColor().g, x->cShape->Circle.getOutlineColor().b));
            }
            else x->cShape->Circle.setOutlineColor(sf::Color(0, x->cShape->Circle.getOutlineColor().g, x->cShape->Circle.getOutlineColor().b));
            if (x->cShape->Circle.getOutlineColor().g - 7 > 0) {
                x->cShape->Circle.setOutlineColor(sf::Color(x->cShape->Circle.getOutlineColor().r, x->cShape->Circle.getOutlineColor().g - 7, x->cShape->Circle.getOutlineColor().b));
            }
            else x->cShape->Circle.setOutlineColor(sf::Color(x->cShape->Circle.getOutlineColor().r, 0, x->cShape->Circle.getOutlineColor().b));
            if (x->cShape->Circle.getOutlineColor().b - 7 > 0) {
                x->cShape->Circle.setOutlineColor(sf::Color(x->cShape->Circle.getOutlineColor().r, x->cShape->Circle.getOutlineColor().g, x->cShape->Circle.getOutlineColor().b - 7));
            }
            else x->cShape->Circle.setOutlineColor(sf::Color(x->cShape->Circle.getOutlineColor().r, x->cShape->Circle.getOutlineColor().g, 0));

            if (x->cShape->Circle.getFillColor().r - 7 > 0) {
                x->cShape->Circle.setFillColor(sf::Color(x->cShape->Circle.getFillColor().r - 7, x->cShape->Circle.getFillColor().g, x->cShape->Circle.getFillColor().b));
            }
            else x->cShape->Circle.setFillColor(sf::Color(0, x->cShape->Circle.getFillColor().g, x->cShape->Circle.getFillColor().b));
            if (x->cShape->Circle.getFillColor().g - 7 > 0) {
                x->cShape->Circle.setFillColor(sf::Color(x->cShape->Circle.getFillColor().r, x->cShape->Circle.getFillColor().g - 7, x->cShape->Circle.getFillColor().b));
            }
            else x->cShape->Circle.setFillColor(sf::Color(x->cShape->Circle.getFillColor().r, 0, x->cShape->Circle.getFillColor().b));
            if (x->cShape->Circle.getFillColor().b - 7 > 0) {
                x->cShape->Circle.setFillColor(sf::Color(x->cShape->Circle.getFillColor().r, x->cShape->Circle.getFillColor().g, x->cShape->Circle.getFillColor().b - 7));
            }
            else x->cShape->Circle.setFillColor(sf::Color(x->cShape->Circle.getFillColor().r, x->cShape->Circle.getFillColor().g, 0));

        }
        if (x->cLifespan->remaining <= 0) {
            g_entitymanager.remove(x);
        }
    }
}

void Game::spawnplayer() {
    auto e = g_entitymanager.addentity("player");
    g_player = e;
    e->cTransform = new CTransform(Vec2((g_window.getSize().x) / 2, (g_window.getSize().y) / 2), Vec2(g_playerconfig.S, g_playerconfig.S));
    e->cShape = new CShape(g_playerconfig.SR, g_playerconfig.V, sf::Color(g_playerconfig.FR, g_playerconfig.FG, g_playerconfig.FB), sf::Color(g_playerconfig.OR, g_playerconfig.OG, g_playerconfig.OB), g_playerconfig.OT);
    e->cCollision = new CCollision(g_playerconfig.CR);
    e->cinput = new CInput();
    e->cScore = new CScore(0);

}

int Game::rand_num(int min, int max) {
    if (min > max) {
        std::cerr << "Error: Invalid range! Minimum value must be less than or equal to maximum value." << std::endl;
        return -1;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(gen);
}



void Game::spawnEnemy() {
    auto e = g_entitymanager.addentity("enemy");
    int color_r = rand_num(100, 200); int color_g = rand_num(100, 200); int color_b = rand_num(100, 200);
    e->cShape = new CShape(g_enemyconfig.SR, rand_num(g_enemyconfig.VMIN, g_enemyconfig.VMAX), sf::Color(color_r, color_g, color_b), sf::Color(color_r + 40, color_g + 40, color_b + 40), g_enemyconfig.OT);
    e->cTransform = new CTransform(Vec2(rand_num(g_enemyconfig.SR, g_window.getSize().x - g_enemyconfig.SR), rand_num(+g_enemyconfig.SR, g_window.getSize().y - g_enemyconfig.SR)), Vec2(rand_num(-g_enemyconfig.SMIN, g_enemyconfig.SMAX), rand_num(-g_enemyconfig.SMIN, g_enemyconfig.SMAX)));
    e->cCollision = new CCollision(g_enemyconfig.SR);
}

void Game::sRender() {
    //clear
    g_window.clear(sf::Color::Black);

    //font
    string s1 = "SCORE :" + to_string((int)g_score);
    g_text.setString(s1);
    string s2 = "CONTROLS \nUP : W\nDOWN : S\nLEFT : A\nRIGHT : D\nFIRE : L MOUSE\nPAUSE : P\nSPECIAL : SPACE";
    g_text_1.setString(s2);
    g_text_1.setPosition(0, g_window.getSize().y - 9 * g_text_1.getCharacterSize() - 5);

    g_text_2.setPosition(g_window.getSize().x - g_text_2.getLocalBounds().width, 0);


    //entity
    for (auto x : g_entitymanager.getentity()) {
        g_window.draw(x->cShape->Circle);
    }
    g_window.draw(g_text);
    g_window.draw(g_text_1);
    g_window.draw(g_text_2);
    g_window.display();
}

void Game::spawnBullets(Entity* entity, Vec2 mousePos) {
    auto e = g_entitymanager.addentity("bullet");
    e->cShape = new  CShape(g_bulletconfig.SR, g_bulletconfig.V, sf::Color(g_bulletconfig.FR, g_bulletconfig.FG, g_bulletconfig.FB), sf::Color(g_bulletconfig.OR, g_bulletconfig.OG, g_bulletconfig.OB), g_bulletconfig.OT);
    float theta = atan((mousePos.y - g_player->cTransform->pos.y) / (mousePos.x - g_player->cTransform->pos.x));
    Vec2 temp = Vec2((mousePos.x - g_player->cTransform->pos.x), (mousePos.y - g_player->cTransform->pos.y)).normalise();
    e->cTransform = new CTransform(((Vec2((mousePos.x - g_player->cTransform->pos.x), (mousePos.y - g_player->cTransform->pos.y)).normalise() ) * g_playerconfig.SR) + Vec2(g_player->cTransform->pos.x, g_player->cTransform->pos.y), Vec2(g_bulletconfig.S * cos(theta) * temp.x, g_bulletconfig.S * abs(sin(theta)) * temp.y));
    e->cCollision = new CCollision(g_bulletconfig.CR);
    e->cLifespan = new CLifespan(g_bulletconfig.L);

}
void Game::spawnSmallEnemies(Entity* entity) {
    float angle = 44.f / (7 * entity->cShape->Circle.getPointCount());
    for (int i = 0; i < entity->cShape->Circle.getPointCount(); i++) {
        auto e = g_entitymanager.addentity("smallenemy");
        e->cShape = new CShape(entity->cShape->Circle.getRadius() / 5, entity->cShape->Circle.getPointCount(), entity->cShape->Circle.getFillColor(), entity->cShape->Circle.getOutlineColor(), entity->cShape->Circle.getOutlineThickness() / 2);
        e->cLifespan = new CLifespan(g_enemyconfig.L);
        e->cTransform = new CTransform(entity->cTransform->pos, Vec2(entity->cTransform->vel.x * cos(i * angle), entity->cTransform->vel.x * sin(i * angle)));

    }
}

void Game::sEnemySpawner() {

    if (g_current_frame > 0) {
        if (g_current_frame % g_enemyconfig.SI == 0) {
            spawnEnemy();
        }
    }
}

void Game::sCollision() {

    for (auto x : g_entitymanager.getentity("bullet")) {
        for (auto y : g_entitymanager.getentity("enemy")) {
            int dx = x->cTransform->pos.x - y->cTransform->pos.x;
            int dy = x->cTransform->pos.y - y->cTransform->pos.y;
            int actual = dx * dx + dy * dy;
            int collision_radius = x->cCollision->radius + y->cCollision->radius;
            if (actual < collision_radius * collision_radius) {
                g_entitymanager.remove(x);
                spawnSmallEnemies(y);
                g_entitymanager.remove(y);
                g_score = g_score + 100;

            }
        }
    }
    for (auto x : g_entitymanager.getentity("special_bullets")) {
        for (auto y : g_entitymanager.getentity("enemy")) {
            int dx = x->cTransform->pos.x - y->cTransform->pos.x;
            int dy = x->cTransform->pos.y - y->cTransform->pos.y;
            int actual = dx * dx + dy * dy;
            int collision_radius = x->cCollision->radius + y->cCollision->radius;
            if (actual < collision_radius * collision_radius) {
                g_entitymanager.remove(x);
                spawnSmallEnemies(y);
                g_entitymanager.remove(y);
                g_score = g_score + 100;

            }
        }
    }

    for (auto x : g_entitymanager.getentity("enemy")) {
        auto y = g_player;
        if (g_entitymanager.getentity("enemy").size() != 0) {
            int dx = x->cTransform->pos.x - y->cTransform->pos.x;
            int dy = x->cTransform->pos.y - y->cTransform->pos.y;
            int actual = dx * dx + dy * dy;
            int collision_radius = x->cCollision->radius + y->cCollision->radius;
            if (actual < collision_radius * collision_radius) {

                g_entitymanager.removeall();
                dead.play();
                bg1.stop();
                bg1.play();
                spawnplayer();
                g_player = g_entitymanager.getentity("player")[0];
                g_score = 0;
                g_text_2.setString("YOU HAVE SPECIAL ! ");
                g_text_2.setFillColor(sf::Color(150, 150, 150));

            }
        }
    }

}

void Game::spawnSpecialWeapon(Entity* entity) {
    //12 different bullets //angle = 30 in degree
    float angle = 30.f * 22.f / (7.f * 180.f);
    special.play();
    for (int i = 0; i < 12; i++) {
        auto e = g_entitymanager.addentity("special_bullets");
        e->cShape = new CShape(g_bulletconfig.SR, g_bulletconfig.V, sf::Color(sf::Color::Black), sf::Color(g_player->cShape->Circle.getOutlineColor()), g_bulletconfig.OT);
        e->cCollision = new CCollision(g_bulletconfig.CR);
        e->cLifespan = new CLifespan(g_bulletconfig.L);
        e->cTransform = new CTransform(Vec2(g_player->cTransform->pos.x + cos(i * angle) * g_player->cShape->Circle.getRadius(), g_player->cTransform->pos.y + sin(i * angle) * g_player->cShape->Circle.getRadius()), Vec2(g_bulletconfig.S * (cos(i * angle)), g_bulletconfig.S * (sin(i * angle))));
    }
}