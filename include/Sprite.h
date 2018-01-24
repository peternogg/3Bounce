#pragma once

#include <3ds.h>
#include <citro3d.h>

#include "Point.h"
#include "Vertex.h"

// A rectangular, textured image to display on screen
class Sprite {
public:
    static int VerticesPerSprite() { return 6; }

    Sprite(const Point& size, const Point& texTopLeft, const Point& texBottomRight, Vertex* bufferArea);
    ~Sprite();

    void MoveBy(Point diff);
    void MoveTo(Point position);


    void Update();
private:
    Point _size; // Holds the width and the height of the sprite
    Point _position; // Top left corner
    Vertex* _vertices; // Sprites are rectangular

    bool _needsUpdate;
};