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

private:
	void init(const QRectF& geometry, QRgb background = qRgb(100, 100, 100));
		
	void processPixel(Vector2d p, double z, QRgb color = Qt::black);
	void processPixel(Vector3d p, QRgb color = Qt::black);
	void processPixel(double x, double y, double z, QRgb color = Qt::black);

	void processLine(Vector3d p1, Vector3d p2, QRgb color = Qt::black);

	shared_ptr<Drawer> drawer = nullptr;
	MatrixX<double> zBuffer;
	MatrixX<QRgb> frameBuffer;
};
