#include "RenderManager.h"
#include "Exceptions/Exceptions.h"

RenderManager::RenderManager() : drawer(nullptr) { }

shared_ptr<Drawer> RenderManager::getDrawer() {
	return drawer;
}

void RenderManager::setDrawer(const shared_ptr<Drawer>& drawer) {
	this->drawer = drawer;
}

void RenderManager::setPixel(Vector2d dp, QRgb color) {
    Vector2i p = dp.cast<int>();
    // TODO: исправить
    // if (!frameBuffer) throw EmptyException(EXCEPCION_ARGS, "Image has't been initialized"); 
    if (p.x() < 0 || p.x() >= frameBuffer.rows() || p.y() < 0 || p.y() >= frameBuffer.cols())
        return;

    // image->setPixelColor(p.x(), p.y(), color);
    frameBuffer(p.x(), p.y()) = color;
}

void RenderManager::setPixel(double x, double y, QRgb color) {
    this->setPixel(Vector2d(x, y), color);
}

void RenderManager::setLine(Vector2d p1, Vector2d p2, QRgb color) {
    double xStart = p1.x(), xEnd = p2.x(), yStart = p1.y(), yEnd = p2.y();

    if (xStart == xEnd && yStart == yEnd) {
        this->setPixel(p1, color);
        return;
    }

    double deltaX = xEnd - xStart;
    double deltaY = yEnd - yStart;

    int trX = abs(deltaX);
    int trY = abs(deltaY);

    int length;
    if (trX > trY)
        length = trX;
    else
        length = trY;

    deltaX /= length;
    deltaY /= length;

    double curX = xStart;
    double curY = yStart;

    for (int i = 0; i < length; i++) {
        this->setPixel(round(curX), round(curY), color);
        curX += deltaX;
        curY += deltaY;
    }
}

void RenderManager::renderScene(const shared_ptr<Scene>& scene, const QRectF& geometry) {
    drawer->initDrawer(geometry);

	if (!scene) {
		drawer->drawImage();
		return;
	}
	// TODO: Сделать через Z буфер

	for (auto& model : scene->getModels()) {
		// Нагрузка для проверки
		for (auto& face : model->getDetails()->getFaces()) {
			for (auto& v : face->getVertices()) {
				auto pos = v->getPosition();
				double test = (pos.x() + pos.y() + pos.z()) * (pos.x() + pos.y() + pos.z());
				// qDebug() << test;
			}
		}
		// Нагрузка для проверки



		Edges edges = model->getDetails()->getEdges();
		// Рисуем все ребра
		for (auto& edge : edges) {
			drawer->setLine(edge->getVertices()[0]->getScreenPosition(), edge->getVertices()[1]->getScreenPosition());
		}
	}
	drawer->drawImage();
}
