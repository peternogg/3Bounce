#pragma once

#include <3ds.h>
#include <citro3d.h>
#include <stdlib.h>
#include <cmath>
#include <vector>

#include "lodepng.h"

#include "ScreenLog.h"
#include "Vertex.h"
#include "Sprite.h"
#include "IGameObject.h"

// Shader info
// Generated at runtime
#include "vshader_shbin.h"
#include "Spritesheet_png.h"

#define COMMON_DISPLAY_TRANSFER_FLAGS (GX_TRANSFER_FLIP_VERT(0)                      |\
                                       GX_TRANSFER_OUT_TILED(0)                      |\
                                       GX_TRANSFER_RAW_COPY(0)                       |\
                                       GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8)  |\
                                       GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8)  |\
                                       GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

#define TEXTURE_TRANSFER_FLAGS (GX_TRANSFER_FLIP_VERT(1)                             |\
                                GX_TRANSFER_OUT_TILED(1)                             |\
                                GX_TRANSFER_RAW_COPY(0)                              |\
	                            GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8)         |\
                                GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGBA8)        |\
	                            GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))


#define BUFFER_CLEAR_COLOR 0x6495EDFF
#define TWO_PI 2.0f * 3.14159f
#define MAX_SPRITES 20

// Adapted + guided by https://github.com/tommai78101/homebrew/wiki/Version-002:-Core-Engine
// and by devkitPro 3DS graphics examples

class Engine {
public:
    static Engine* Instance() { return _instance; }

    Engine(ScreenLog* log);
    ~Engine();
    
    void InitializeGraphics();

    void Update();
    void Draw();

    void AddObject(IGameObject* object);
    Sprite* CreateSprite(const Vector2& size, const Vector2& sheetTL, const Vector2& sheetBL);
    
    bool ButtonDown(u32 button);
private:
    static Engine* _instance;

    ScreenLog* _log;

    // Drawing/Graphics stuff
    C3D_RenderTarget* _target;
    DVLB_s* _shaderBinary;
    Vertex* _VBO;

    shaderProgram_s _shaderProgram;
    s8 _projectionInputHandle;
    C3D_Mtx _projectionMatrix;
    C3D_Tex _spritesheet;

    // Drawn frames since game start
    u64 _frameCount;
    // The number of sprites in use
    u8 _spriteCount;
    
    // Objects in the engine
    std::vector<IGameObject*> _objects;

    void LoadSpritesheet();
};

