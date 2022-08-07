#pragma once
#include <fstream>
#include <sstream>
#include "Scene/Scene.h"

using namespace std;

using Models = std::vector<std::shared_ptr<Model>>;

class Loader
{
public:
	Loader();
	Loader(string srcFilename);

	void openFile(const std::string& src_name);
	void closeFile();

	// Метод для загрузки только моделей из файла .obj
	Models loadModels();

	// Метод для загрузки всей сцены из файла .sol
	shared_ptr<Scene> loadScene();

private:
	Vector3d readPosition(istringstream &lineStream);
	vector<int> readFaceDetails(istringstream &lineStream);

	shared_ptr<Edge> addEdgeToModel(Edges &modelEdges, const shared_ptr<Edge> &newEdge);

	std::shared_ptr<std::ifstream> srcFile;
};

