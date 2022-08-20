#include "Drawer.h"
#include "Exceptions/Exceptions.h"

Drawer::Drawer(shared_ptr<QGraphicsScene> graphicsScene) : graphicsScene(graphicsScene) { }

shared_ptr<QGraphicsScene> Drawer::getGraphicsScene() {
	return graphicsScene;
}

void Drawer::setGraphicsScene(const shared_ptr<QGraphicsScene>& graphicsScene) {
	this->graphicsScene = graphicsScene;
}

void Drawer::initDrawer(const QRectF& geometry) {
    graphicsScene->setSceneRect(geometry);
    // frameBuffer.resize(geometry.width(), geometry.height());
    // frameBuffer.fill(background.rgba());
    // image = make_shared<QImage>(geometry.width(), geometry.height(), QImage::Format_RGB32);
    // image->fill(background);
}

void Drawer::clearScene() {
	graphicsScene->clear();
}

void Drawer::drawScene(MatrixX<QRgb> frameBuffer) {
    shared_ptr<QImage> image = make_shared<QImage>(frameBuffer.rows(), frameBuffer.cols(), QImage::Format_RGB32);

    for (int y = 0; y < image->height(); ++y) {
        QRgb* line = reinterpret_cast<QRgb*>(image->scanLine(y));
        for (int x = 0; x < image->width(); ++x) {
            QRgb& pixel = line[x];
            pixel = frameBuffer(y, x);
        }
    }
	graphicsScene->addPixmap(QPixmap::fromImage(*image));
}
