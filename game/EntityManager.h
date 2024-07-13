#pragma once
#include "Entity.h"
typedef std::vector <Entity* > Entityvec;
typedef std::map <std::string, Entityvec> Entitymap;

class EntityManager {

    Entityvec entity_vec;
    Entitymap entity_map;
    Entityvec to_add;
    size_t totalentity = 0;

public:

    EntityManager() {};
    void update();
    Entity* addentity(const std::string tag);
    Entityvec& getentity(); //all
    Entityvec& getentity(const std::string tag);
    void remove(Entity*& e);
    void removeall();

};