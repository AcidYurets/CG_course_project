#include <qDebug>
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
    data = (uchar*) frameBuffer.data();
    QImage image = QImage(frameBuffer.rows(), frameBuffer.cols(), QImage::Format_RGB32);

    /*for (int y = 0; y < image.height(); ++y) {
        QRgb* line = reinterpret_cast<QRgb*>(image.scanLine(y));
        for (int x = 0; x < image.width(); ++x) {
            QRgb& pixel = line[x];
            pixel = frameBuffer(x, y);
            // qDebug() << "x = " << x << " y = " << y << "pixel = " << pixel;
        }
    }*/ 
    
    image.setPixel(QPoint(5, 5), qRgb(100, 100, 100));
    qDebug() << image.pixel(QPoint(5, 5)) << image.pixel(QPoint(5, 6));
    image.save("QImage.png", "PNG");
	graphicsScene->addPixmap(QPixmap::fromImage(image));
}
