#pragma once
#include "Scene/Scene.h"

class SelectionManager
{
public:
	void selectModel(shared_ptr<Scene>& scene, Vector2i mousePos);
	shared_ptr<Model> getSelectedModel();

private:
	shared_ptr<Model> selectedModel;

	Faces selectedFaces = Faces(0);
	Edges selectedEdges = Edges(0);
	Vertices selectedVertices = Vertices(0);
};

