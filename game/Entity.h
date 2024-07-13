#pragma once
#pragma once
#include "Component.h"
#include <string>
using namespace std;

class Entity {

    friend class EntityManager;

    bool e_alive = true;
    unsigned int e_id = 0;
    string e_tag = "default";

public:
    CTransform* cTransform = NULL;
    CShape* cShape = NULL;
    CCollision* cCollision = NULL;
    CInput* cinput = NULL;
    CScore* cScore = NULL;
    CLifespan* cLifespan = NULL;

    bool isActive() const;
    std::string tag();
    unsigned int id();
    void destroy();

};
