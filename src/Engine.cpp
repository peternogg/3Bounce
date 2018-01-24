#include "Engine.h"

Engine::Engine(ScreenLog* log) 
    : _log(log), _target(nullptr), _frameCount(0), _spriteBuffer(nullptr), _sprite(nullptr) {

    Point spriteSize = { 100, 100 };
    Point middlePosition = { 400 / 2 - 50, 240 / 2 - 50};

    _log->PrintLine("Engine created");
    
    // Get data for sprite vertices in linear ram
    _spriteBuffer = (Vertex*)linearAlloc(sizeof(Vertex) * Sprite::VerticesPerSprite());

    _sprite = new Sprite(spriteSize, {0, 0}, {1, 1}, _spriteBuffer);
    _sprite->MoveTo(middlePosition);
}

Engine::~Engine() {
    delete _sprite;
    linearFree(_spriteBuffer);

    _sprite = nullptr;
    _spriteBuffer = nullptr;
}

void Engine::InitializeGraphics() {
    C3D_BufInfo* bufferInfo;
    C3D_AttrInfo* attrInfo;
    C3D_TexEnv* textureEnvironment;
    int code;

    _log->PrintLine("Initializing 3D Graphics...");

    // Initialize Citro 3D
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    // Create the render target (where the rendered pixels go)
    _target = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
    // Set the render clearing parameters
    C3D_RenderTargetSetClear(_target, C3D_CLEAR_ALL, BUFFER_CLEAR_COLOR, 0);
    // Set render outputs
    C3D_RenderTargetSetOutput(_target, GFX_TOP, GFX_LEFT, COMMON_DISPLAY_TRANSFER_FLAGS);

    _log->PrintLine("Initialized 3D Graphics");
    _log->PrintLine("Initializing Scene...");

    // Load up the shader binary (generated during build)
    _shaderBinary = DVLB_ParseFile((u32*)vshader_shbin, vshader_shbin_size);

    // Create a shader program
    shaderProgramInit(&_shaderProgram);
    shaderProgramSetVsh(&_shaderProgram, _shaderBinary->DVLE);

    // Bind the program to the GPU
    C3D_BindProgram(&_shaderProgram);

    // Get the location of the "projection" input in the vertex shader
    _projectionInputHandle = shaderInstanceGetUniformLocation(_shaderProgram.vertexShader, "projection");

    // Tell the GPU and shader what the information it's gonna get is
    attrInfo = C3D_GetAttrInfo();
    AttrInfo_Init(attrInfo);
    // These map to the inputs in the vshader
    // The second argument is regId - Register ID
    // 0 -> v0, 1 -> v1, etc
    // So the line here is "v0 will recieve a 3 floats" which are placed into
    // v0's x, y, and z components
    AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 2); // v0 = Vertex position
    //AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2); // Vertex coords
    AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2); // v1 = Vertex Color

    Mtx_OrthoTilt(&_projectionMatrix, 0.0, 400.0, 240.0, 0.0, 0.0, 1.0, true);

    // Configure fragment shader to pass color thru
    textureEnvironment = C3D_GetTexEnv(0);
    C3D_TexEnvSrc(textureEnvironment, C3D_Both, GPU_PRIMARY_COLOR, 0, 0);
    C3D_TexEnvOp(textureEnvironment, C3D_Both, 0, 0, 0);
    C3D_TexEnvFunc(textureEnvironment, C3D_Both, GPU_REPLACE);

    bufferInfo = C3D_GetBufInfo();
    BufInfo_Init(bufferInfo);

    code = BufInfo_Add(bufferInfo, _spriteBuffer, sizeof(Vertex), 2, 0x10);

    if (code < 0)
        _log->PrintLine("!! ERROR !!: BufInfo_Add returned < 0");

    _log->PrintLine("Initialized scene");
}

void Engine::Update() {
    // Move the sprite in a circle around the screen
    _spriteAngle += TWO_PI / 120; // Rotate in a circle every ~2s (assuming 60fps)

    Point diff = { std::cos(2 * _spriteAngle), std::sin(_spriteAngle) };

    _sprite->MoveBy(diff);
}




void Engine::Draw() {
    ++_frameCount;

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C3D_FrameDrawOn(_target);

    // Update uniforms
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, _projectionInputHandle, &_projectionMatrix);

    // C3D_ImmDrawBegin(GPU_TRIANGLES);
    // {
    //     for(int i = 0; i < 3; i++) 
    //     {
    //         C3D_ImmSendAttrib(vertex_list[i].x, vertex_list[i].y, vertex_list[i].z, 0);
    //         C3D_ImmSendAttrib(vertex_list[i].r, vertex_list[i].g, vertex_list[i].b, 1.0f);
    //     }
    // }
    // C3D_ImmDrawEnd();

    C3D_DrawArrays(GPU_TRIANGLES, 0, sizeof(Vertex) * Sprite::VerticesPerSprite());

    C3D_FrameEnd(0);
}