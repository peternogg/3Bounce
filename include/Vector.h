#pragma once

struct Vector2 {
    float x, y;

    Vector2(float x, float y) :
        x(x), y(y)
    {

    }

    Vector2& operator+=(const Vector2& other) {
        this->x += other.x;
        this->y += other.y;

        return *this;
    }

    Vector2& operator-=(const Vector2& other) {
        this->x -= other.x;
        this->y -= other.y;

        return *this;
    }

    friend Vector2 operator+(const Vector2& left, const Vector2& right) {
        return Vector2(left.x + right.x, left.y + right.y); 
    }

    friend Vector2 operator-(const Vector2& left, const Vector2& right) {
        return Vector2(left.x - right.x, left.y - right.y); 
    }
};