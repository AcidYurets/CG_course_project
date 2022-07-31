#pragma once
#include <string>
#include <fstream>
using namespace std;

class SceneLoader
{
public:
	SceneLoader(string srcFilename);

	loadModel();
	loadLightSource();
	loadCamera();

private:
	std::shared_ptr<std::ifstream> srcFile;
};

