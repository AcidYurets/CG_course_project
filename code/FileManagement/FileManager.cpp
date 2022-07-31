#include "FileManager.h"
#include "SceneLoader/SceneLoader.h"

shared_ptr<Scene> FileManager::loadScene(std::string srcFilename)
{
	SceneLoader loader(srcFilename);

	shared_ptr<Scene> scene = make_shared<Scene>();
}
