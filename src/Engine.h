#pragma once

#include <3ds.h>
#include <citro3d.h>
#include <stdlib.h>

#include <algorithm> // std::min

#include "ScreenLog.h"

// Shader info
// Generated at runtime
#include "vshader_shbin.h"

#define COMMON_DISPLAY_TRANSFER_FLAGS (GX_TRANSFER_FLIP_VERT(0) 					 |\
									   GX_TRANSFER_OUT_TILED(0) 					 |\
									   GX_TRANSFER_RAW_COPY(0)                       |\
									   GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8)  |\
									   GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8)  |\
									   GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

#define BUFFER_CLEAR_COLOR 0x6495EDFF

// Adapted + guided by https://github.com/tommai78101/homebrew/wiki/Version-002:-Core-Engine
// and by devkitPro 3DS graphics examples

typedef struct { float x, y, z; float r, g, b; } vertex;

class Engine {
public:
	Engine(ScreenLog* log);
	~Engine();

	void Update();
	void Draw();

	void InitializeGraphics();

private:
	ScreenLog* _log;
	C3D_RenderTarget* _target;
	DVLB_s* _shaderBinary;
	shaderProgram_s _shaderProgram;
	s8 _projectionInputHandle;
	u64 _frameCount;
	C3D_Mtx _projectionMatrix;

	vertex vertex_list[3] = {
		{ 200.0f, 200.0f, 0.5f, 0.0f, 0.0f, 0.0f },
		{ 100.0f, 40.0f, 0.5f,  0.0f, 0.0f, 0.0f },
		{ 300.0f, 40.0f, 0.5f,  0.0f, 0.0f, 0.0f },
	};
};
