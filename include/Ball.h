#pragma once

#include <3ds.h>

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
    Sprite* _sprite;

    Point _position;
    Point _size;
    Point _velocity;
};
