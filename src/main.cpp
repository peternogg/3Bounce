#include <3ds.h>

#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "ScreenLog.h"
#include "Engine.h"
#include "IGameObject.h"
#include "Ball.h"

using std::cout;
using std::ifstream;

int main(int argc, char** argv) {
    // Setup
    gfxInitDefault();

    srand(100);

    ScreenLog log;
    Engine engine(&log);
    engine.InitializeGraphics();
    
    Ball ball;

    bool shouldQuit = false;

    engine.AddObject(&ball);

    while(aptMainLoop() && !shouldQuit) {

        hidScanInput();
        if (KEY_START & hidKeysDown())
            shouldQuit = true;

        engine.Update();
        engine.Draw();
    }

    gfxExit();
}