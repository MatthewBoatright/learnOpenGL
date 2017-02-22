#include "fileUtils.h"

namespace fileUtilities {

	std::string getFileContents(const char* filePath)
	{
		std::string content;

		try {
			std::ifstream fileStream(filePath, std::ios::in);

			if (!fileStream.is_open())
			{
				std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
				return "";
			}

			std::string line = "";
			while (!fileStream.eof())
			{
				std::getline(fileStream, line);
				content.append(line + "\n");
			}

			fileStream.close();
		}
		catch(std::ifstream::failure e){
			std::cout << "ERROR::FILE::NOT_SUCCESFULLY_READ" << std::endl;
			content = "";
		}

		return content;
	}

}