#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include <iostream>
#include <fstream>

namespace fileUtilities {
	std::string getFileContents(const char* filePath);
}

#endif