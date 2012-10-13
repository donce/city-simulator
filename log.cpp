#include "log.h"

void Log(const char *text)
{
	std::ofstream f("log.txt", std::fstream::app);

	f << text << std::endl;

	f.close();
}