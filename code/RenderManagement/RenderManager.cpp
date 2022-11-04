#include "RenderManager.h"
#include "../Exceptions/Exceptions.h"

#define MAX_DEPTH INT_MIN
#define EPS 1e-5

RenderManager::RenderManager() { }

void RenderManager::initImage(shared_ptr<QImage> image) {
	this->frameBuffer = image;
}

shared_ptr<Face> RenderManager::getFaceBufferValue(int x, int y) {
	return faceBuffer(x, y);
}

MatrixX<shared_ptr<Face>> RenderManager::getFaceBuffer() {
	return faceBuffer;
}

bool RenderManager::getPerspective() {
	return isPerspective;
}

void RenderManager::setPerspective(bool isPerspective) {
	this->isPerspective = isPerspective;
}

void RenderManager::renderScene(const shared_ptr<Scene>& scene, const QRectF& geometry) {
	if (!scene) {
		return;
	}

	this->initBuffers(geometry);
	QPainter qPainter = QPainter(frameBuffer.get());

	// Сначала рисуем оси координат
	auto xLine = Line(Vertex(-10, 0, 0), Vertex(10, 0, 0));
	this->processLine(xLine.p1.getScreenPosition(scene->getCamera(), isPerspective, Vector2d(geometry.center().x(), geometry.center().y())),
		xLine.p2.getScreenPosition(scene->getCamera(), isPerspective, Vector2d(geometry.center().x(), geometry.center().y())), QColor(Qt::red).rgba());
	auto yLine = Line(Vertex(0, -10, 0), Vertex(0, 10, 0));
	this->processLine(yLine.p1.getScreenPosition(scene->getCamera(), isPerspective, Vector2d(geometry.center().x(), geometry.center().y())),
		yLine.p2.getScreenPosition(scene->getCamera(), isPerspective, Vector2d(geometry.center().x(), geometry.center().y())), QColor(Qt::green).rgba());
	auto zLine = Line(Vertex(0, 0, -10), Vertex(0, 0, 10));
	this->processLine(zLine.p1.getScreenPosition(scene->getCamera(), isPerspective, Vector2d(geometry.center().x(), geometry.center().y())),
		zLine.p2.getScreenPosition(scene->getCamera(), isPerspective, Vector2d(geometry.center().x(), geometry.center().y())), QColor(Qt::blue).rgba());

	for (auto& model : scene->getModels()) {
		// Отображаем грани
		for (auto& face : model->getDetails()->getFaces()) {
			renderFace(face, scene, Vector2d(geometry.center().x(), geometry.center().y()));
		}

		// Отображаем ребра
		for (auto& edge : model->getDetails()->getEdges()) {
			this->processLine(edge->getVertices()[0]->getScreenPosition(scene->getCamera(), isPerspective, Vector2d(geometry.center().x(), geometry.center().y())),
				edge->getVertices()[1]->getScreenPosition(scene->getCamera(), isPerspective, Vector2d(geometry.center().x(), geometry.center().y())));
		}

		// Отображаем вершины
		for (auto& vertex : model->getDetails()->getVertices()) {
			auto screenPos = vertex->getScreenPosition(scene->getCamera(), isPerspective, Vector2d(geometry.center().x(), geometry.center().y()));
			if (checkPixel(screenPos.x(), screenPos.y(), screenPos.z() + 2)) {
				auto pen = QPen(Qt::black);
				auto brush = QBrush(Qt::black);
				int r = 2;
				if (vertex->selected) {
					brush = QBrush(Qt::cyan);
					r = 3;
				}

				qPainter.setBrush(brush);
				qPainter.setPen(pen);
				qPainter.drawEllipse(QPointF(screenPos.x(), screenPos.y()), r, r);
			}
		}
	}
}


void RenderManager::initBuffers(const QRectF& geometry, QRgb background) {
	zBuffer.resize(geometry.width(), geometry.height());
	zBuffer.fill(MAX_DEPTH); // TODO: Установить нормальное значение заполнения

	QImage image(geometry.width(), geometry.height(), QImage::Format_RGB32);
	*frameBuffer = image;
	frameBuffer->fill(background);

	faceBuffer.resize(geometry.width(), geometry.height());
	faceBuffer.fill(nullptr);
}

void RenderManager::renderFace(const shared_ptr<Face>& face, const shared_ptr<Scene>& scene, Vector2d screenCenter) {
	if (face == nullptr)
		throw EmptyException(EXCEPCION_ARGS, "Face is null");

	// Проверка, что грань не лицевая
	auto normal = face->getNormal(scene->getCamera(), isPerspective, screenCenter);
	auto check = normal.dot(Vector3d(0, 0, 1));
	//qDebug() << "Face x:" << check;
	if (check < 0) return;

	ScreenFace screenFace(0);
	for (auto& v : face->getVertices()) {
		screenFace.push_back(v->getScreenPosition(scene->getCamera(), isPerspective, screenCenter));
	}
	
	// Вычисляем обрамляющий прямоугольник
	auto framingRect = this->calculateFramingRect(screenFace);
	
	// Вычисляем цвет грани (он один, т.к. используется простой алгоритм закраски)
	auto faceColor = this->calculateFaceColor(face, scene->getCamera(), isPerspective, screenCenter);

	// Обрабатываем все приксели обр. прямоугольника
	this->processFace(screenFace, framingRect, faceColor, face);
}

void RenderManager::processPixel(Vector2d p, double z, QRgb color) {
	if (!frameBuffer) throw EmptyException(EXCEPCION_ARGS, "Image has't been initialized");

	Vector2i roundedP = p.cast<int>();
	if (!frameBuffer->valid(roundedP.x(), roundedP.y())) 
		return;

	if (z >= zBuffer(roundedP.x(), roundedP.y()))
	{
		zBuffer(roundedP.x(), roundedP.y()) = z;
		frameBuffer->setPixel(roundedP.x(), roundedP.y(), color);
	}
}

void RenderManager::processPixel(Vector3d p, QRgb color) {
	this->processPixel(Vector2d(p.x(), p.y()), p.z(), color);
}

void RenderManager::processPixel(double x, double y, double z, QRgb color) {
	this->processPixel(Vector2d(x, y), z, color);
}

void RenderManager::processLine(Vector3d p1, Vector3d p2, QRgb color) {
	double xStart = p1.x(), xEnd = p2.x(), yStart = p1.y(), yEnd = p2.y(), zStart = p1.z(), zEnd = p2.z();

	if (xStart == xEnd && yStart == yEnd) {
		if (zStart > zEnd)
			this->processPixel(p1, color);
		else
			this->processPixel(p2, color);
		return;
	}

	double deltaX = xEnd - xStart;
	double deltaY = yEnd - yStart;
	double deltaZ = zEnd - zStart;

	int trX = abs(deltaX);
	int trY = abs(deltaY);
	int trZ = abs(deltaZ);

	int length = max({ trX, trY });

	deltaX /= length;
	deltaY /= length;
	deltaZ /= length;

	double curX = xStart;
	double curY = yStart;
	double curZ = zStart;

	for (int i = 0; i < length; i++) {
		// TODO: Почему именно 1?
		this->processPixel(curX, curY, curZ + 1, color);
		curX += deltaX;
		curY += deltaY;
		curZ += deltaZ;
	}
}

void RenderManager::processFace(const ScreenFace& face, const QRect& framingRect, const QRgb& color, const shared_ptr<Face>& basicFace) {
	double square = (face[0].y() - face[2].y()) * (face[1].x() - face[2].x()) +
		(face[1].y() - face[2].y()) * (face[2].x() - face[0].x());

	for (int y = framingRect.top(); y <= framingRect.bottom(); y++) {
		for (int x = framingRect.left(); x <= framingRect.right(); x++) {
			auto barCoords = calculateBarycentric(QPoint(x, y), face, square);

			if (barCoords.x() >= -EPS && barCoords.y() >= -EPS && barCoords.z() >= -EPS) {
				double z = baryCentricInterpolation(face[0], face[1], face[2], barCoords);
				
				this->processPixel(x, y, z, color);
				this->updateFaceBuffer(Vector2d(x, y), z, basicFace);
			}
		}
	}
}

bool RenderManager::checkPixel(Vector2d p, double z) {
	if (!frameBuffer) throw EmptyException(EXCEPCION_ARGS, "Image has't been initialized");

	auto isVisible = false;
	
	Vector2i roundedP = p.cast<int>();
	if (!frameBuffer->valid(roundedP.x(), roundedP.y()))
		return false;

	if (z >= zBuffer(roundedP.x(), roundedP.y())) {
		isVisible = true;
	}

	return isVisible;
}

bool RenderManager::checkPixel(Vector3d p) {
	return this->checkPixel(Vector2d(p.x(), p.y()), p.z());
}

bool RenderManager::checkPixel(double x, double y, double z) {
	return this->checkPixel(Vector2d(x, y), z);
}

QRgb RenderManager::calculateFaceColor(const shared_ptr<Face>& face, const shared_ptr<Camera>& camera, bool isPerspective, Vector2d screenCenter) {
	// TODO: Используя простой алгоритм закраски посчитать цвет грани
	auto n = face->getNormal(camera, isPerspective, screenCenter);
	double k = 130;
	QColor baseColor(face->getColor());
	return baseColor.lighter(k * n.dot(Vector3d(0, 0, 1))).rgb();
}

QRect RenderManager::calculateFramingRect(const ScreenFace& face) {
	Vector3d point = face[0];
	double maxX = point.x(), maxY = point.y();
	double minX = point.x(), minY = point.y();

	for (auto& v : face)
	{
		point = v;
		if (point.x() > maxX) maxX = point.x();
		if (point.y() > maxY) maxY = point.y();

		if (point.x() < minX) minX = point.x();
		if (point.y() < minY) minY = point.y();
	}

	return QRect(QPoint(minX, minY), QPoint(maxX, maxY));
}

Vector3d RenderManager::calculateBarycentric(const QPoint& p, const ScreenFace& triangle, const double& square)
{
	Vector3d barCoords(0, 0, 0);

	barCoords.x() = ((p.y() - triangle[2].y()) * (triangle[1].x() - triangle[2].x()) + (triangle[1].y() - triangle[2].y()) * (triangle[2].x() - p.x())) / square;
	barCoords.y() = ((p.y() - triangle[0].y()) * (triangle[2].x() - triangle[0].x()) + (triangle[2].y() - triangle[0].y()) * (triangle[0].x() - p.x())) / square;
	barCoords.z() = 1 - barCoords.x() - barCoords.y();

	return barCoords;
}

double RenderManager::baryCentricInterpolation(const Vector3d& a, const Vector3d& b, const Vector3d& c, const Vector3d& bary)
{
	double z = bary.x() * a.z() + bary.y() * b.z() + bary.z() * c.z();
	return z;
}

// Обновляет значение в буфере граней
void RenderManager::updateFaceBuffer(Vector2d p, double z, shared_ptr<Face> face) {
	if (this->checkPixel(p, z)) {
		Vector2i roundedP = p.cast<int>();
		faceBuffer(roundedP.x(), roundedP.y()) = face;
	}
}
