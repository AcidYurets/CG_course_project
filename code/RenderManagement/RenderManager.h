#pragma once
#include "Scene/Scene.h"
#include "Drawer/Drawer.h"

class RenderManager
{
public:
	RenderManager();

	shared_ptr<Drawer> getDrawer();
	void setDrawer(const shared_ptr<Drawer>& drawer);

	void renderScene(const shared_ptr<Scene> &scene, const QRectF& geometry);

	void setPixel(Vector2d p, QRgb color = Qt::black);
	void setPixel(double x, double y, QRgb color = Qt::black);

	// Временный метод. Для основной программы он не нужен
	void setLine(Vector2d p1, Vector2d p2, QRgb color = Qt::black);


private:
	shared_ptr<Drawer> drawer = nullptr;
	MatrixX<QRgb> frameBuffer = MatrixX<QRgb>();
};
