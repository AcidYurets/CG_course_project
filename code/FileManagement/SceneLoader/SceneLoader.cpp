#include "SceneLoader.h"
#include "Exceptions/Exceptions.h"

SceneLoader::SceneLoader(string srcFilename)
{
	this->srcFile = std::make_shared<std::ifstream>(srcFilename);

	if (!*(this->srcFile)) throw FileOpenException(EXCEPCION_ARGS, "Could not open source file");
}
