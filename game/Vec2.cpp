#include "Vec2.h"
#include <math.h>

bool Vec2::operator == (const Vec2 a) {
    if (this->x == a.x && this->y == a.y) return 1;
    else return 0;
}
bool Vec2::operator != (const Vec2 a) {
    if (this->x != a.x || this->y != a.y) return 1;
    else return 0;
}

Vec2 Vec2::operator + (const Vec2& a) {
    Vec2 temp;
    temp.x = this->x + a.x;
    temp.y = this->y + a.y;
    return temp;
}

Vec2 Vec2::operator - (const Vec2& a) {
    Vec2 temp;
    temp.x = this->x - a.x;
    temp.y = this->y - a.y;
    return temp;
}

Vec2 Vec2::operator / (const float a) {
    Vec2 temp;
    temp.x = this->x / a;
    temp.y = this->y / a;
    return temp;
}

Vec2 Vec2::operator * (const float a) {
    Vec2 temp;
    temp.x = this->x * a;
    temp.y = this->y * a;
    return temp;
}

void Vec2::operator += (const Vec2& a) {
    this->x += a.x;
    this->y += a.y;
}

void Vec2::operator -= (const Vec2& a) {
    this->x -= a.x;
    this->y -= a.y;
}

void Vec2::operator /= (const float a) {
    this->x /= a;
    this->y /= a;
}

void Vec2::operator *= (const float a) {
    this->x *= a;
    this->y *= a;
}

float Vec2::length() {
    return sqrt(this->x * this->x + this->y * this->y);
}

Vec2 Vec2::normalise() {
    float len = this->length();
    this->x = this->x / len;
    this->y = this->y / len;

    Vec2 temp(this->x, this->y);
    return  temp;
}


