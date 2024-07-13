#pragma once
#include "Vec2.h"
#include <SFML\Graphics.hpp>

class CTransform {
public:

    Vec2 pos;  
    Vec2 vel;
    float angle = 0;

    CTransform(const Vec2& a, const Vec2& b) : pos(a), vel(b), angle(0) {}
    CTransform(const Vec2& a, const int b, float c) : pos(a), angle(c) {
        //vel = Vec2(b * cos(c), b * sin(c));
    }

};

class CShape {
public:
    sf::CircleShape Circle;
    CShape(float radius, float sides, sf::Color fill_color, sf::Color outline_color, float outline_thickness) {
        Circle.setRadius(radius);
        Circle.setPointCount(sides);
        Circle.setFillColor(fill_color);
        Circle.setOutlineColor(outline_color);
        Circle.setOutlineThickness(outline_thickness);
        Circle.setOrigin(radius, radius);
        Circle.setRotation(2);
        Circle.setScale(-1, 1);
    }

};

//class CShape {
//public:
//    sf::RectangleShape rect;
//    CShape() {
//    }
//};

class CCollision {
public:
    float radius;
    CCollision(float a) : radius(a) {}

};

class CInput {

public:

    bool up = false;
    bool down = false;;
    bool right = 0;
    bool left = 0;
    bool shoot = 0;
    CInput() {};
};

class CScore {
public:
    float score;
    CScore(float a) :score(a) {}
};

class CLifespan {
public:
    int remaining = 0;
    int total = 0;
    CLifespan(int total) :remaining(total), total(total) {}
};
