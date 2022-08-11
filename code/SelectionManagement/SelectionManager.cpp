#include "SelectionManager.h"
#include "Exceptions/Exceptions.h"

void SelectionManager::selectModel(shared_ptr<Scene> &scene, Vector2i mousePos) {
	// TODO: Реализовать нормальный выбор модели
	if (!scene) throw EmptyException(EXCEPCION_ARGS, "Scene was't created");
	if (scene->getModels().empty()) throw EmptyException(EXCEPCION_ARGS, "Models is empty");
    selectedModel = scene->getModels()[0];
	// scene->setSelectedItems(...)
}

shared_ptr<Model> SelectionManager::getSelectedModel() {
	return selectedModel;
}
