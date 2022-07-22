#pragma once
#include <string>

class FileManager
{
public:
	Scene loadScene(std::string src);
	void saveScene(std::string dst, Scene scene);

};

