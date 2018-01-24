#include "Sprite.h"

#include <stdio.h>

Sprite::Sprite(const Point& size, const Point& texTopLeft, const Point& texBottomRight, Vertex* bufferArea) 
    : _size(size), _position({0, 0}), _vertices(bufferArea)
{
    float texWidth = texBottomRight.x - texTopLeft.x;
    float texHeight = texBottomRight.y - texTopLeft.y;

    // Triangle 1
    _vertices[0] = { 0, 0, texTopLeft.x, texTopLeft.y };
    _vertices[1] = { 0, size.y, texTopLeft.x, texTopLeft.y + texHeight };
    _vertices[2] = { size.x, 0, texTopLeft.x + texWidth, texTopLeft.x };

    // Triangle 2
    _vertices[3] = _vertices[1];
    _vertices[4] = { size.x, size.y, texTopLeft.x + texWidth, texTopLeft.y + texHeight };
    _vertices[5] = _vertices[2];
}

void Sprite::MoveBy(Point diff) {
    _position.x += diff.x;
    _position.y += diff.y;

    for (int i = 0; i < VerticesPerSprite(); i++) {
        _vertices[i].x += diff.x;
        _vertices[i].y += diff.y;
    }
}

void Sprite::MoveTo(Point position) {
    // Find the new position relative to the current position
    Point diff = position - _position;
    // Move by the diff to update each vertex
    MoveBy(diff);
}

void Sprite::Update() {

}

Sprite::~Sprite() {
    // Do nothing
}

