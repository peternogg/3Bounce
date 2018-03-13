#include "Sprite.h"

Sprite::Sprite(const Vector2& size, const Vector2& texTopLeft, const Vector2& texBottomRight, Vertex* bufferArea) 
    : _size(size), _position(0, 0), _vertices(bufferArea)
{
    // Triangle 1
    _vertices[0] = { 0, 0, texTopLeft.x, texTopLeft.y };
    _vertices[1] = { 0, size.y, texTopLeft.x, texBottomRight.y };
    _vertices[2] = { size.x, 0, texBottomRight.x, texTopLeft.y };

    // Triangle 2
    _vertices[3] = _vertices[1];
    _vertices[4] = { size.x, size.y, texBottomRight.x, texBottomRight.y };
    _vertices[5] = _vertices[2];
}

void Sprite::MoveBy(Vector2 diff) {
    _position.x += diff.x;
    _position.y += diff.y;

    for (int i = 0; i < VerticesPerSprite(); i++) {
        _vertices[i].x += diff.x;
        _vertices[i].y += diff.y;
    }
}

void Sprite::MoveTo(Vector2 position) {
    // Find the new position relative to the current position
    Vector2 diff = position - _position;
    // Move by the diff to update each vertex
    MoveBy(diff);
}

void Sprite::Update() {

}

Sprite::~Sprite() {
    // Do nothing
}

