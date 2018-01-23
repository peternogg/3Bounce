#include "Engine.h"

Engine::Engine(ScreenLog* log) 
    : _log(log), _target(nullptr), _frameCount(0), _vertex_buffer(nullptr) {
    _log->PrintLine("Engine created");
    
    // Copy data from vertex_list to _vertex_buffer in linear RAM
    _vertex_buffer = (vertex*)linearAlloc(sizeof(vertex_list));
    memcpy(_vertex_buffer, vertex_list, sizeof(vertex_list));
}

Engine::~Engine() {
    // Destroy _log
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
    AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0 = Vertex position
    //AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2); // Vertex coords
    AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 3); // v1 = Vertex Color

    Mtx_OrthoTilt(&_projectionMatrix, 0.0, 400.0, 0.0, 240.0, 0.0, 1.0, true);

    // Configure fragment shader to pass color thru
    textureEnvironment = C3D_GetTexEnv(0);
    C3D_TexEnvSrc(textureEnvironment, C3D_Both, GPU_PRIMARY_COLOR, 0, 0);
    C3D_TexEnvOp(textureEnvironment, C3D_Both, 0, 0, 0);
    C3D_TexEnvFunc(textureEnvironment, C3D_Both, GPU_REPLACE);

    bufferInfo = C3D_GetBufInfo();
    BufInfo_Init(bufferInfo);

    code = BufInfo_Add(bufferInfo, _vertex_buffer, sizeof(vertex), 2, 0x10);

    if (code < 0)
        _log->PrintLine("!! ERROR !!: BufInfo_Add returned < 0");

    _log->PrintLine("Initialized scene");
}

void Engine::Update() {
    //_log->PrintLine("Updating...");

    // Make the colors change slowly
    for (int i = 0; i < 3; i++) {
        _vertex_buffer[i].r += 0.001f;
        if (_vertex_buffer[i].r > 1.0f) _vertex_buffer[i].r = 0.0f;

        _vertex_buffer[i].g += 0.001f;
        if (_vertex_buffer[i].g > 1.0f) _vertex_buffer[i].g = 0.0f;

        _vertex_buffer[i].b += 0.001f;
        if (_vertex_buffer[i].b > 1.0f) _vertex_buffer[i].b = 0.0f;
    }

    if (rand() % 100 == 0) {
        _log->PrintLine("Messing with colors!");
        // Choose a vertex and increase its color values
        int chosenVertex = rand() % 3;
        _vertex_buffer[chosenVertex].r *= 1.1;
        _vertex_buffer[chosenVertex].g *= 1.2;
        _vertex_buffer[chosenVertex].b *= 1.3;
    }

    // Move the top vertex across the screen
    _vertex_buffer[0].x -= 1.0f;
    if (_vertex_buffer[0].x <= 0)
        _vertex_buffer[0].x = 400.0f;
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

    C3D_DrawArrays(GPU_TRIANGLES, 0, sizeof(vertex_list)); // _vertex_buffer is as big as vertex_list

    C3D_FrameEnd(0);
}