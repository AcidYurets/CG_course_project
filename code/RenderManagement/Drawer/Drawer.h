#pragma once
#include "QGraphicsScene"
#include <memory>
#include <Eigen/Dense>
using namespace std;
using namespace Eigen;

class Drawer
{
public:
	Drawer(shared_ptr<QGraphicsScene> graphicsScene);

	shared_ptr<QGraphicsScene> getGraphicsScene();
	void setGraphicsScene(const shared_ptr<QGraphicsScene>& graphicsScene);
	void initDrawer(const QRectF& geometry);

	void clearScene();

	// Отобразить frameBuffer на QGraphicsScene
	void drawScene(MatrixX<QRgb> frameBuffer);

private:
	shared_ptr<QGraphicsScene> graphicsScene = nullptr;
	// shared_ptr<QImage> image = nullptr;
};

