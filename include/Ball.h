#pragma once

#include <3ds.h>

#include "Vector.h"
#include "Engine.h"
#include "Sprite.h"
#include "IGameObject.h"

class Ball : public IGameObject {
public:
    Ball();
    ~Ball();

    void Update();
    void Draw();
private:
    Vector2 _position;
    Vector2 _size;
    Vector2 _velocity;

    Sprite* _sprite;
};
