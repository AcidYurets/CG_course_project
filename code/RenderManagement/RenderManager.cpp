#include "RenderManager.h"
#include "Exceptions/Exceptions.h"

void RenderManager::renderScene(QGraphicsScene* graphicsScene, const shared_ptr<Scene>& scene) {
	if (!scene) throw EmptyException(EXCEPCION_ARGS, "Scene was't created");
	graphicsScene->clear();

	// TODO: Сделать через Z буфер
	for (auto& model : scene->getModels()) {
		Edges edges = model->getDetails()->getEdges();
		// Рисуем все ребра
		for (auto& edge : edges) {
			QLineF line(edge->getVertices()[0]->getScreenPosition().x(), edge->getVertices()[0]->getScreenPosition().y(), 
						edge->getVertices()[1]->getScreenPosition().x(), edge->getVertices()[1]->getScreenPosition().y());
			graphicsScene->addLine(line);
		}
	}
}
