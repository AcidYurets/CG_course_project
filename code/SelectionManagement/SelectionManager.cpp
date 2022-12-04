#include "SelectionManager.h"
#include "../Exceptions/Exceptions.h"



SelectionManager::SelectionManager() : selectedModel(nullptr), selectedFaces(0), selectedEdges(0), selectedVertices(0) { }

void SelectionManager::setFaceBuffer(MatrixX<shared_ptr<Face>> faceBuffer) {
	this->faceBuffer = faceBuffer;
}

void SelectionManager::setModelBuffer(MatrixX<shared_ptr<Model>> modelBuffer) {
	this->modelBuffer = modelBuffer;
}

bool SelectionManager::selectLightSources(shared_ptr<Scene>& scene, Vector2i mousePos) {
	if (!scene) throw EmptyException(EXCEPCION_ARGS, "Scene was't created");
	if (scene->getModels().empty()) throw EmptyException(EXCEPCION_ARGS, "Models is empty");

	auto &lightSources = scene->getLightSources();
	if (lightSources.size() == 0) return false;

	for (auto &ls : lightSources) {
		auto pos = ls->getPosition().getScreenPosition(scene->getCamera(), isPerspective, screenCenter);
		auto dist = getDistance2D(Vector2d(mousePos.x(), mousePos.y()), Vector2d(pos.x(), pos.y()));
		if (dist < 10) {
			selectedLightSources.push_back(ls);
			ls->selected = true;
			return true;
		}
	}

	return false;
}

void SelectionManager::selectModel(shared_ptr<Scene> &scene, Vector2i mousePos) {
	if (!scene) throw EmptyException(EXCEPCION_ARGS, "Scene was't created");
	if (scene->getModels().empty()) throw EmptyException(EXCEPCION_ARGS, "Models is empty");

	auto& model = modelBuffer(mousePos.x(), mousePos.y());
	if (model == nullptr) return;

    this->selectedModel = model;
	model->selected = true;
}

void SelectionManager::selectFace(shared_ptr<Scene>& scene, Vector2i mousePos) {
	if (!scene) throw EmptyException(EXCEPCION_ARGS, "Scene was't created");
	if (scene->getModels().empty()) throw EmptyException(EXCEPCION_ARGS, "Models is empty");

	auto& face = faceBuffer(mousePos.x(), mousePos.y());
	if (face == nullptr) return;

	// Выбираем саму грань
	selectedFaces.push_back(face);
	face->selected = true;

	// Выбираем вершины и ребра грани
	auto edges = face->getEdges();
	for (size_t i = 0; i < edges.size(); i++) {
		selectedEdges.push_back(edges[i]);
		edges[i]->selected = true;
	}

	auto vertices = face->getVertices();
	for (size_t i = 0; i < vertices.size(); i++) {
		selectedVertices.push_back(vertices[i]);
		vertices[i]->selected = true;
	}
}

void SelectionManager::selectEdge(shared_ptr<Scene>& scene, Vector2i mousePos) {
	if (!scene) throw EmptyException(EXCEPCION_ARGS, "Scene was't created");
	if (scene->getModels().empty()) throw EmptyException(EXCEPCION_ARGS, "Models is empty");

	auto& face = faceBuffer(mousePos.x(), mousePos.y());
	if (face == nullptr) return;

	double minDist = INT_MAX;
	shared_ptr<Edge> chosenEdge;
	for (auto& e : face->getEdges()) {
		auto v1Pos = e->getVertices()[0]->getScreenPosition(scene->getCamera(), isPerspective, screenCenter);
		auto v2Pos = e->getVertices()[1]->getScreenPosition(scene->getCamera(), isPerspective, screenCenter);
		double dist = getDistToLine2D(Vector2d(mousePos.x(), mousePos.y()), Vector2d(v1Pos.x(), v1Pos.y()), Vector2d(v2Pos.x(), v2Pos.y()));
		if (dist < minDist) {
			minDist = dist;
			chosenEdge = e;
		}
	}

	// Выбираем само ребро
	selectedEdges.push_back(chosenEdge);
	chosenEdge->selected = true;

	// Выбираем вершины ребра
	selectedVertices.push_back(chosenEdge->getVertices()[0]);
	selectedVertices.push_back(chosenEdge->getVertices()[1]);
	chosenEdge->getVertices()[0]->selected = true;
	chosenEdge->getVertices()[1]->selected = true;
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
	selectedLightSources.clear();

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
	for (auto& ls : scene->getLightSources()) {
		ls->selected = false;
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

LightSources SelectionManager::getSelectedLightSources() {
	return selectedLightSources;
}
