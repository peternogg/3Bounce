#include <3ds.h>

#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "ScreenLog.h"
#include "Engine.h"

using std::cout;
using std::ifstream;

int main(int argc, char** argv) {
    // Setup
    gfxInitDefault();

    srand(100);

    ScreenLog log;
    Engine engine(&log);

    bool shouldQuit = false;

    engine.InitializeGraphics();

    while(aptMainLoop() && !shouldQuit) {

        hidScanInput();
        if (KEY_START & hidKeysDown())
            shouldQuit = true;

        engine.Update();
        engine.Draw();
    }

    gfxExit();
}