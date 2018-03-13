#include "Engine.h"

Engine* Engine::_instance = nullptr;

Engine::Engine(ScreenLog* log) 
    : _log(log), _target(nullptr), _VBO(nullptr), _spritesheet(), _frameCount(0), _spriteCount(0), _objects() {

    if (_instance == nullptr)
        _instance = this;

    _log->PrintLine("Engine created");
    
    // Get data for sprite vertices in linear ram
    _VBO = (Vertex*)linearAlloc(sizeof(Vertex) * Sprite::VerticesPerSprite() * MAX_SPRITES);
}

Engine::~Engine() {
    linearFree(_VBO);

    _VBO = nullptr;
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
    AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2); // v1 = Texture coordinates

    Mtx_OrthoTilt(&_projectionMatrix, 0.0, 400.0, 240.0, 0.0, 0.0, 1.0, true);

    _log->PrintLine("Loading spritesheet...");

    LoadSpritesheet();

    _log->PrintLine("Loaded spritesheet");

    // Configure fragment shader to pass color thru
    textureEnvironment = C3D_GetTexEnv(0);
    C3D_TexEnvSrc(textureEnvironment, C3D_Both, 
        GPU_TEXTURE0,
        GPU_FRAGMENT_PRIMARY_COLOR, 
        GPU_PRIMARY_COLOR);

    C3D_TexEnvOp(textureEnvironment, C3D_Both, 
        GPU_TEVOP_RGB_SRC_COLOR, 
        GPU_TEVOP_RGB_SRC_COLOR, 
        GPU_TEVOP_RGB_SRC_COLOR);

    C3D_TexEnvFunc(textureEnvironment, C3D_Both, GPU_BLEND_ADD);

    bufferInfo = C3D_GetBufInfo();
    BufInfo_Init(bufferInfo);

    code = BufInfo_Add(bufferInfo, _VBO, sizeof(Vertex), 2, 0x10);

    //C3D_DepthTest(true, GPU_TESTFUNC::GPU_EQUAL, GPU_WRITEMASK::GPU_WRITE_ALL);

    if (code < 0)
        _log->PrintLine("!! ERROR !!: BufInfo_Add returned <0");

    _log->PrintLine("Initialized scene");
}

void Engine::Update() {
    for (IGameObject* obj : _objects)
        obj->Update();
}

void Engine::Draw() {
    ++_frameCount;

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C3D_FrameDrawOn(_target);

    // Update uniforms
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, _projectionInputHandle, &_projectionMatrix);

    // Draw the sprites
    // Primitive, first index, count indicies
    C3D_DrawArrays(GPU_TRIANGLES, 0, sizeof(Vertex) * Sprite::VerticesPerSprite() * _spriteCount);

    C3D_FrameEnd(0);
}

void Engine::AddObject(IGameObject* obj) {
    if (_objects.size() < MAX_SPRITES)
        _objects.push_back(obj);
}

Sprite* Engine::CreateSprite(const Vector2& size, const Vector2& sheetTL, const Vector2& sheetBL) {
    Vertex* spriteEntry;
    Sprite* sprite = nullptr;

    if (_spriteCount < MAX_SPRITES) {
        // Get the memory for the sprite
        spriteEntry = _VBO + (_spriteCount * Sprite::VerticesPerSprite());
        sprite = new Sprite(size, sheetTL, sheetBL, spriteEntry);

        _spriteCount++;
    }

    return sprite;
}

void Engine::LoadSpritesheet() {
    // Adapted from 3DS gpusprites example
    // Load spritesheet from Spritesheet_png
    u8* spritesheetData;
    unsigned int width, height;
    u8* gpu_spriteMemory;

    // Decode the spritesheet
    if (lodepng_decode32(&spritesheetData, &width, &height, Spritesheet_png, Spritesheet_png_size) != 0) {
        _log->PrintLine("!! PROBLEM: lodepng failed !!");
        return;
    }

    // Get w * h * 4 bytes per pixel of memory in a place
    // the GPU likes
    gpu_spriteMemory = (u8*)linearAlloc(width * height * 4);

    // Rearrange the bytes for the 3ds
    u8 r, g, b, a;
    u8* src = spritesheetData;
    u8* dest = gpu_spriteMemory;
    
    // This looks like it reverses the order of the bytes in memory
    for (u32 i = 0; i < width * height; i++) {
        r = *src++;
        g = *src++;
        b = *src++;
        a = *src++;

        *dest++ = a;
        *dest++ = b;
        *dest++ = g;
        *dest++ = r;
    }

    GSPGPU_FlushDataCache(gpu_spriteMemory, width * height * 4);

    C3D_TexInit(&_spritesheet, width, height, GPU_RGBA8);
    C3D_SafeDisplayTransfer((u32*)gpu_spriteMemory, GX_BUFFER_DIM(width, height), (u32*)_spritesheet.data, GX_BUFFER_DIM(width, height), TEXTURE_TRANSFER_FLAGS);

    gspWaitForPPF();

    C3D_TexSetFilter(&_spritesheet, GPU_LINEAR, GPU_NEAREST);
    
    // Put the spritesheet on texture unit 1
    C3D_TexBind(0, &_spritesheet);

    // And clean up!
    free(spritesheetData);
    linearFree(gpu_spriteMemory);
}

bool Engine::ButtonDown(u32 button) {
    return button & hidKeysDown();
}