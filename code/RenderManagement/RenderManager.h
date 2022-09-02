#pragma once
#include "Scene/Scene.h"
#include "Drawer/Drawer.h"

class RenderManager
{
public:
	RenderManager();

	void initImage(shared_ptr<QImage> image);

	// Рендерит сцену во frameBuffer
	void renderScene(const shared_ptr<Scene> &scene, const QRectF& geometry);

private:
	void initBuffers(const QRectF& geometry, QRgb background = qRgb(100, 100, 100));
		
	void processPixel(Vector2d p, double z, QRgb color = Qt::black);
	void processPixel(Vector3d p, QRgb color = Qt::black);
	void processPixel(double x, double y, double z, QRgb color = Qt::black);

	void processLine(Vector3d p1, Vector3d p2, QRgb color = Qt::black);

	shared_ptr<Drawer> drawer = nullptr;
	MatrixX<double> zBuffer;
	shared_ptr<QImage> frameBuffer;
};
