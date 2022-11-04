#include "SelectionManager.h"
#include "../Exceptions/Exceptions.h"



SelectionManager::SelectionManager() : selectedModel(nullptr), selectedFaces(0), selectedEdges(0), selectedVertices(0) { }

void SelectionManager::setFaceBuffer(MatrixX<shared_ptr<Face>> faceBuffer) {
	this->faceBuffer = faceBuffer;
}

void SelectionManager::selectModel(shared_ptr<Scene> &scene, Vector2i mousePos) {
	if (!scene) throw EmptyException(EXCEPCION_ARGS, "Scene was't created");
	if (scene->getModels().empty()) throw EmptyException(EXCEPCION_ARGS, "Models is empty");

	// TODO: Реализовать нормальный выбор модели
    selectedModel = scene->getModels()[0];
	scene->getModels()[0]->selected = true;
	// scene->setSelectedItems(...)
}

void SelectionManager::selectFace(shared_ptr<Scene>& scene, Vector2i mousePos) {
}

void SelectionManager::selectEdge(shared_ptr<Scene>& scene, Vector2i mousePos) {
}


void SelectionManager::selectVertex(shared_ptr<Scene>& scene, Vector2i mousePos) {
	if (!scene) throw EmptyException(EXCEPCION_ARGS, "Scene was't created");
	if (scene->getModels().empty()) throw EmptyException(EXCEPCION_ARGS, "Models is empty");

	auto &face = faceBuffer(mousePos.x(), mousePos.y());
	if (face == nullptr) return;

	double minDist = INT_MAX;
	shared_ptr<Vertex> chosenVertex;
	for (auto& v : face->getVertices()) {
		auto vPos = v->getScreenPosition(scene->getCamera(), isPerspective, screenCenter);
		double dist = getDistance2D(Vector2d(vPos.x(), vPos.y()), Vector2d(mousePos.x(), mousePos.y()));
		if (dist < minDist) {
			minDist = dist;
			chosenVertex = v;
		}
	}

	selectedVertices.push_back(chosenVertex);
	chosenVertex->selected = true;
}

void SelectionManager::clearSelecteds(shared_ptr<Scene>& scene) {
	selectedVertices.clear();
	selectedEdges.clear();
	selectedFaces.clear();

	for (auto& m : scene->getModels()) {
		m->selected = false;
		auto d = m->getDetails();
		for (auto& f : d->getFaces()) {
			f->selected = false;
		}
		for (auto& e : d->getEdges()) {
			e->selected = false;
		}
		for (auto& v : d->getVertices()) {
			v->selected = false;
		}
	}
}

shared_ptr<Model> SelectionManager::getSelectedModel() {
	return selectedModel;
}

Faces SelectionManager::getSelectedFaces() {
	return selectedFaces;
}

Edges SelectionManager::getSelectedEdges() {
	return selectedEdges;
}

Vertices SelectionManager::getSelectedVertices() {
	return selectedVertices;
}
