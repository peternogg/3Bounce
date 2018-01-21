#include "ScreenLog.h"

ScreenLog::ScreenLog() {
	// Initialize lower screen for console output
	consoleInit(GFX_BOTTOM, &_console);

	this->PrintLine("Log initialized");
}

ScreenLog::~ScreenLog() {

}

void ScreenLog::Print(std::string message) {
	std::cout << message;
}

void ScreenLog::Print(const char* message) {
	std::cout << message;
}

void ScreenLog::PrintLine(std::string message) {
	this->Print(message);
	std::cout << '\n';
}

void ScreenLog::PrintLine(const char* message) {
	this->Print(message);
	std::cout << '\n';
}