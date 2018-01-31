#pragma once

class IGameObject {
public:
    IGameObject() {};
    virtual ~IGameObject() {};

    virtual void Update() = 0;
    virtual void Draw() = 0;
};
