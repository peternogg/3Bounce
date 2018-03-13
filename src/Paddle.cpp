#include "Paddle.h"

Paddle::Paddle() 
    : _size(50, 150), _position(20, 240 / 2 - (_size.y / 2))
{
    _sprite = Engine::Instance()->CreateSprite(
        _size,
        {0, 0}, 
        {40.f / 128.f, 120.f / 128.f}
    );
}

Paddle::~Paddle() {

}

void Paddle::Update() {
    u32 keys = hidKeysDown() | hidKeysHeld();

    if (keys & KEY_DOWN) { 
        _position.y += 1;
    }

    if (keys & KEY_UP) {
        _position.y -= 1;
    }

    _sprite->MoveTo(_position);
}

void Paddle::Draw() {

}