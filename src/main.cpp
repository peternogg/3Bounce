#include <3ds.h>

#include <iostream>
#include <fstream>

using std::cout;
using std::ifstream;

int main(int argc, char** argv) {
	// Setup
	gfxInitDefault();
	consoleInit(GFX_TOP, nullptr);

	bool shouldQuit = false;

	std::ifstream input("file.txt");
	char c;

	if (input.is_open()) {
		std::cout << "input open\n";
		std::cout << "file.txt contents:\n";

		// Copy chars from input to stdout
		while(input.good()) {
			c = input.get();
			std::cout.put(c);
		}
	} else {
		std::cout << "file bad\n";
	}

	input.close();

	while(aptMainLoop() && !shouldQuit) {

		hidScanInput();
		if (KEY_START & hidKeysDown())
			shouldQuit = true;

	}

	gfxExit();
}