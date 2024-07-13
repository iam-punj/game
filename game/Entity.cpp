#include "Entity.h"
using namespace std;

bool Entity::isActive() const {
    if (this->e_alive == true) return 1;
    else return 0;
}

string Entity::tag() {
    return this->e_tag;
}

unsigned int Entity::id() {
    return this->e_id;
}

void Entity::destroy() {
    this->e_alive = false;
}