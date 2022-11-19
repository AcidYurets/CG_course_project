#pragma once
#include "../Scene/Scene.h"
#include "../Consts/Consts.h"
#include "../Objects/Line/Line.h"
#include <QWidget>
#include <QPainter>
#include <thread>

using ScreenFace = std::vector<Vector3d>;

struct ThreadParams {
	int startY, stopY;
	int startX, stopX;
	const ScreenFace face;
	double square;
	const QRgb color;
	const shared_ptr<Face> basicFace;
	const shared_ptr<Model> model;
};

class RenderManager
{
public:
	// 0 - объектный режим
	// 1 - режим редактирования граней
	// 2 - режим редактирования ребер
	// 3 - режим редактирования вершин
	int mode = objectMode;

	RenderManager();

	void initImage(shared_ptr<QImage> image);

	shared_ptr<Face> getFaceBufferValue(int x, int y);
	MatrixX<shared_ptr<Face>> getFaceBuffer();
	shared_ptr<Model> getModelBufferValue(int x, int y);
	MatrixX<shared_ptr<Model>> getModelBuffer();

	bool getPerspective();
	void setPerspective(bool isPerspective);

	// Рендерит сцену во frameBuffer
	void renderScene(const shared_ptr<Scene> &scene, const QRectF& geometry);

private:
	// Inits
	void initBuffers(const QRectF& geometry, QRgb background = qRgb(180, 180, 180));

	// Renders
	void renderFace(const shared_ptr<Face>& face, const shared_ptr<Scene>& scene, Vector2d screenCenter, const shared_ptr<Model>& model);
	
	// Processes
	void processPixel(Vector2d p, double z, QRgb color = Qt::black);
	void processPixel(Vector3d p, QRgb color = Qt::black);
	void processPixel(double x, double y, double z, QRgb color = Qt::black);
	void processLine(Vector3d p1, Vector3d p2, QRgb color = Qt::black);
	void processFace(const ScreenFace& face, const QRect& framingRect, const QRgb& color, const shared_ptr<Face>& basicFace, const shared_ptr<Model>& model);
	void processFramingRect(unique_ptr<ThreadParams> params);

	bool checkPixel(Vector2d p, double z);
	bool checkPixel(Vector3d p);
	bool checkPixel(double x, double y, double z);

	// Colors
	QRgb calculateFaceColor(const shared_ptr<Face>& face, const shared_ptr<Camera>& camera, bool isPerspective, Vector2d screenCenter);

	// Other calcs
	QRect calculateFramingRect(const ScreenFace& face);
	Vector3d calculateBarycentric(const QPoint& p, const ScreenFace& triangle, const double& square);
	double baryCentricInterpolation(const Vector3d& a, const Vector3d& b, const Vector3d& c, const Vector3d& bary);

	// Обновляет значение в буфере граней
	void updateFaceBuffer(Vector2d p, double z, shared_ptr<Face> face);
	// Обновляет значение в буфере моделей
	void updateModelBuffer(Vector2d p, double z, shared_ptr<Model> model);

	void colorSelectedVertex(QPainter& qPainter, Vector3d point);

	MatrixX<double> zBuffer;
	shared_ptr<QImage> frameBuffer;
	MatrixX<shared_ptr<Face>> faceBuffer;
	MatrixX<shared_ptr<Model>> modelBuffer;

	vector<vector<shared_ptr<Face>>> test1 = vector<vector<shared_ptr<Face>>>(10000, vector<shared_ptr<Face>>(10000));
	vector<vector<double>> test2 = vector<vector<double>>(10000, vector<double>(10000));
	vector<vector<double>> test3 = vector<vector<double>>(10000, vector<double>(10000));

	bool isPerspective = true;
};
