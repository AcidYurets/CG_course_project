#pragma once
#include <string>
#include "../Scene/Scene.h"
using namespace std;

class FileManager
{
public:
	shared_ptr<Scene> loadScene(std::string srcFilename);
	Models loadModels(std::string srcFilename);

	void saveScene(std::string dstFilename, shared_ptr<Scene> scene);
};
