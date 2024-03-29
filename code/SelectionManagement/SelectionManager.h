#pragma once
#include "../Scene/Scene.h"

class SelectionManager
{
public:
	// Поле, дублирующее значение аналогичного у RenderManager
	bool isPerspective = true;
	// Поле, дублирующее значение аналогичного у MainWindow
	Vector2d screenCenter;

	SelectionManager();

	void setFaceBuffer(MatrixX<shared_ptr<Face>> faceBuffer);
	void setModelBuffer(MatrixX<shared_ptr<Model>> modelBuffer);

	void selectModel(shared_ptr<Scene>& scene, Vector2i mousePos);
	void selectFace(shared_ptr<Scene>& scene, Vector2i mousePos);
	void selectEdge(shared_ptr<Scene>& scene, Vector2i mousePos);
	void selectVertex(shared_ptr<Scene>& scene, Vector2i mousePos);
	bool selectLightSources(shared_ptr<Scene>& scene, Vector2i mousePos);

	void clearSelecteds(shared_ptr<Scene>& scene);

	shared_ptr<Model> getSelectedModel();
	Faces getSelectedFaces();
	Edges getSelectedEdges();
	Vertices getSelectedVertices();
	LightSources getSelectedLightSources();

private:
	MatrixX<shared_ptr<Face>> faceBuffer;
	MatrixX<shared_ptr<Model>> modelBuffer;

	shared_ptr<Model> selectedModel = nullptr;

	Faces selectedFaces = Faces(0);
	Edges selectedEdges = Edges(0);
	Vertices selectedVertices = Vertices(0);

	LightSources selectedLightSources = LightSources(0);
};

