#pragma once

typedef struct Point {
    float x, y;

    friend struct Point operator-(const Point& left, const Point& right) {
        return {
            left.x - right.x,
            left.y - right.y
        };
    }

    Point& operator+=(const Point& other) {
        x += other.x;
        y += other.y;

        return *this;
    }

    friend struct Point operator+(const Point& left, const Point& right) {
        return {
            left.x + right.x,
            left.y + right.y
        };
    }
} Point;