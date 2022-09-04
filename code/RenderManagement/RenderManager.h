#pragma once
#include "Scene/Scene.h"
#include <QWidget>

class RenderManager
{
public:
	RenderManager();

	void initImage(shared_ptr<QImage> image);

	bool getPerspective();
	void setPerspective(bool isPerspective);

	// Рендерит сцену во frameBuffer
	void renderScene(const shared_ptr<Scene> &scene, const QRectF& geometry);

private:
	void initBuffers(const QRectF& geometry, QRgb background = qRgb(180, 180, 180));
		
	void processPixel(Vector2d p, double z, QRgb color = Qt::black);
	void processPixel(Vector3d p, QRgb color = Qt::black);
	void processPixel(double x, double y, double z, QRgb color = Qt::black);

	void processLine(Vector3d p1, Vector3d p2, QRgb color = Qt::black);

	MatrixX<double> zBuffer;
	shared_ptr<QImage> frameBuffer;

	bool isPerspective = true;
};
