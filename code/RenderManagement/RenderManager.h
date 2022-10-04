#pragma once
#include "../Scene/Scene.h"
#include <QWidget>
#include <QPainter>

using ScreenFace = std::vector<Vector3d>;

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
	// Inits
	void initBuffers(const QRectF& geometry, QRgb background = qRgb(180, 180, 180));

	// Renders
	void renderFace(const shared_ptr<Face>& face, const shared_ptr<Scene>& scene, Vector2d screenCenter);
	
	// Processes
	void processPixel(Vector2d p, double z, QRgb color = Qt::black);
	void processPixel(Vector3d p, QRgb color = Qt::black);
	void processPixel(double x, double y, double z, QRgb color = Qt::black);
	void processLine(Vector3d p1, Vector3d p2, QRgb color = Qt::black);
	void processFace(const ScreenFace& face, const QRect& framingRect, const QRgb& color);

	bool checkPixel(Vector2d p, double z);
	bool checkPixel(Vector3d p);
	bool checkPixel(double x, double y, double z);

	// Colors
	QRgb calculateFaceColor(const shared_ptr<Face>& face, const shared_ptr<Camera>& camera, bool isPerspective, Vector2d screenCenter);

	// Other calcs
	QRect calculateFramingRect(const ScreenFace& face);
	Vector3d calculateBarycentric(const QPoint& p, const ScreenFace& triangle, const double& square);
	double baryCentricInterpolation(const Vector3d& a, const Vector3d& b, const Vector3d& c, const Vector3d& bary);

	MatrixX<double> zBuffer;
	shared_ptr<QImage> frameBuffer;

	bool isPerspective = true;
};
