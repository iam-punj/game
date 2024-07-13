#pragma once
class Vec2 {
public:
    float x; float y;

    Vec2() {};
    Vec2(float a, float b) : x(a), y(b) {}
    bool operator == (const Vec2 a);
    bool operator != (const Vec2 b);

    Vec2 operator + (const Vec2& a);
    Vec2 operator - (const Vec2& a);
    Vec2 operator * (const float a);
    Vec2 operator / (const float a);

    void operator += (const Vec2& a);
    void operator -= (const Vec2& a);
    void operator *= (const float a);
    void operator /= (const float a);

    float  length();
    Vec2 normalise();
};
