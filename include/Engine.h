#pragma once

#include <3ds.h>
#include <citro3d.h>
#include <stdlib.h>
#include <cmath>

#include "ScreenLog.h"
#include "Point.h"
#include "Vertex.h"
#include "Sprite.h"

// Shader info
// Generated at runtime
#include "vshader_shbin.h"

#define COMMON_DISPLAY_TRANSFER_FLAGS (GX_TRANSFER_FLIP_VERT(0)                      |\
                                       GX_TRANSFER_OUT_TILED(0)                      |\
                                       GX_TRANSFER_RAW_COPY(0)                       |\
                                       GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8)  |\
                                       GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8)  |\
                                       GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

#define BUFFER_CLEAR_COLOR 0x6495EDFF
#define TWO_PI 2.0f * 3.14159f

// Adapted + guided by https://github.com/tommai78101/homebrew/wiki/Version-002:-Core-Engine
// and by devkitPro 3DS graphics examples

class Engine {
public:
    Engine(ScreenLog* log);
    ~Engine();

    void Update();
    void Draw();

    void InitializeGraphics();
private:
    ScreenLog* _log;

    // Drawing/Graphics stuff
    C3D_RenderTarget* _target;
    DVLB_s* _shaderBinary;
    shaderProgram_s _shaderProgram;
    s8 _projectionInputHandle;
    C3D_Mtx _projectionMatrix;
    
    // Frames since first draw
    u64 _frameCount;
    
    Vertex* _spriteBuffer;
    Sprite* _sprite;
    float _spriteAngle;
};

