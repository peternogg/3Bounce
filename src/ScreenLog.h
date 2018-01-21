#pragma once

#include <3ds.h>
#include <iostream>

class ScreenLog {
public:
	ScreenLog();
	~ScreenLog();

	void Print(std::string message);
	void Print(const char* message);
	void PrintLine(std::string message);
	void PrintLine(const char* message);
private:
	PrintConsole _console;
};