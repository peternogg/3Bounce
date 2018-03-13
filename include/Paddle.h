#pragma once

#include "Vector.h"
#include "Engine.h"
#include "Sprite.h"
#include "IGameObject.h"

class Paddle : public IGameObject {
public:
    Paddle();
    ~Paddle();

    void Update();
    void Draw();
private:
    Vector2 _size;
    Vector2 _position;

    Sprite* _sprite;
};
