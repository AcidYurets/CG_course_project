#include "RenderManager.h"
#include "Exceptions/Exceptions.h"

RenderManager::RenderManager() : drawer(nullptr) { }

shared_ptr<Drawer> RenderManager::getDrawer() {
	return drawer;
}

void RenderManager::setDrawer(const shared_ptr<Drawer>& drawer) {
	this->drawer = drawer;
}

void RenderManager::renderScene(const shared_ptr<Scene>& scene, const QRectF& geometry) {
    this->init(geometry);

    if (!scene) {
        drawer->drawScene(frameBuffer);
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
            this->processLine(edge->getVertices()[0]->getScreenPosition(scene->getCamera()),
                edge->getVertices()[1]->getScreenPosition(scene->getCamera()));
        }
    }
    drawer->drawScene(frameBuffer);
}


void RenderManager::init(const QRectF& geometry, QRgb background) {
    zBuffer.resize(geometry.width(), geometry.height());
    zBuffer.fill(-1); // TODO: Установить нормальное значение заполнения

    frameBuffer.resize(geometry.width(), geometry.height());
    frameBuffer.fill(background);

    drawer->initDrawer(geometry);
}

void RenderManager::processPixel(Vector2d p, double z, QRgb color) {
    Vector2i roundedP = p.cast<int>();
    // TODO: исправить
    // if (!frameBuffer) throw EmptyException(EXCEPCION_ARGS, "Image has't been initialized"); 
    if (roundedP.x() < 0 || roundedP.x() >= frameBuffer.rows() 
        || roundedP.y() < 0 || roundedP.y() >= frameBuffer.cols())
        return;

    frameBuffer(roundedP.x(), roundedP.y()) = color;
}

void RenderManager::processPixel(Vector3d p, QRgb color) {
    this->processPixel(Vector2d(p.x(), p.y()), p.z(), color);
}

void RenderManager::processPixel(double x, double y, double z, QRgb color) {
    this->processPixel(Vector2d(x, y), z, color);
}

void RenderManager::processLine(Vector3d p1, Vector3d p2, QRgb color) {
    double xStart = p1.x(), xEnd = p2.x(), yStart = p1.y(), yEnd = p2.y();

    if (xStart == xEnd && yStart == yEnd) {
        this->processPixel(p1, color);
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
        // TODO: Посчитать координату z
        this->processPixel(round(curX), round(curY), 0, color);
        curX += deltaX;
        curY += deltaY;
    }
}

