#pragma once

#include <3ds.h>
#include <citro3d.h>

#include "Point.h"
#include "Vertex.h"
#include "Vector.h"

// A rectangular, textured image to display on screen
class Sprite {
public:
    static int VerticesPerSprite() { return 6; }

    Sprite(const Vector2& size, const Vector2& texTopLeft, const Vector2& texBottomRight, Vertex* bufferArea);
    ~Sprite();

    void MoveBy(Vector2 diff);
    void MoveTo(Vector2 position);


    void Update();
private:
    Vector2 _size; // Holds the width and the height of the sprite
    Vector2 _position; // Top left corner
    Vertex* _vertices;
};