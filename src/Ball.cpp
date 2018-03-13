#include "Ball.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240

Ball::Ball() 
    : _position({80, 80}), _size({40, 40}), _velocity({1, 1})
{
    _sprite = Engine::Instance()->CreateSprite(
        _size, 
        {40.f / 128.f, 0.f},  // Sprite sheet TL
        {(40.f + 64.f) / 128.f, (0.f + 64.f) / 128.f} // Sprite sheet TR
    );
    
    _sprite->MoveTo(_position);
}

Ball::~Ball() {

}

void Ball::Update() {
    // Move the ball
    _position += _velocity;

    // Move the sprite
    _sprite->MoveTo(_position);

    // Check collisions
    if (_position.x <= 0 || _position.x + _size.x >= SCREEN_WIDTH)
        _velocity.x = -_velocity.x;

    if (_position.y <= 0 || _position.y + _size.y >= SCREEN_HEIGHT)
        _velocity.y = -_velocity.y;
}

void Ball::Draw() {
    
}