#include "EntityManager.h"
#include <iostream>
Entity* EntityManager::addentity(const std::string tag) {
    Entity* new1 = new Entity();
    new1->e_id = totalentity++;
    new1->e_tag = tag;
    if (tag == "player") {
        entity_vec.push_back(new1); entity_map[tag].push_back(new1);
    }
    else to_add.push_back(new1);
    return new1;

}
void EntityManager::remove(Entity*& e) {

    e->destroy();
}
void EntityManager::update() {

    auto removeIt = std::remove_if(entity_vec.begin(), entity_vec.end(), [](Entity* e)->bool {if (e->e_alive) return 0; else return 1;  });
    entity_vec.erase(removeIt, entity_vec.end());

    for (auto& x : entity_map) {
        auto removeit = std::remove_if(x.second.begin(), x.second.end(), [](Entity* e)->bool {if (e->e_alive) return 0; else return 1; });
        x.second.erase(removeit, x.second.end());
    }

    for (auto& new1 : to_add) {
        entity_vec.push_back(new1);
        entity_map[new1->e_tag].push_back(new1);
    }
    to_add.clear();
}
Entityvec& EntityManager::getentity() {
    return entity_vec;
}
Entityvec& EntityManager::getentity(const std::string tag) {
    return entity_map[tag];
}

void EntityManager::removeall() {
    entity_vec.clear();
    entity_map.clear();
    to_add.clear();
    totalentity = 0;
}