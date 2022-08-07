#include "FileManager.h"
#include "Loader/Loader.h"

shared_ptr<Scene> FileManager::loadScene(std::string srcFilename) {
	Loader loader(srcFilename);

	shared_ptr<Scene> scene = make_shared<Scene>();
	return scene;
}

Models FileManager::loadModels(std::string srcFilename) {
	Loader loader(srcFilename);

	Models models = loader.loadModels();
	return models;
}
