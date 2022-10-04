#include "Vertex.h"
#include "../../Camera/Camera.h"

Vertex::Vertex() : position(0, 0, 0), transMatrix(Matrix4d::Identity()) {
	// Масштабируем и перемещаем в центр экрана
	// this->scale(Vector3d(100, 100, 100));
	// TODO: Найти центр исходя из размера окна
	this->move(Vector3d(370, 280, 0));
	
}

Vertex::Vertex(Vector3d pos) : Vertex() {
	position = pos * 100;
}

Vertex::Vertex(double x, double y, double z) : Vertex(Vector3d(x, y, z)) { }

/*Vector3d Vertex::getPosition() {
	return position;
}*/

void Vertex::setPosition(Vector3d pos) {
	position = pos;
	transMatrix = Matrix4d::Identity();
}

Vector3d Vertex::getTransformPosition() {
	Vector3d res;
	Vector4d beforeTransform;
	beforeTransform << position, 1;

	Vector4d afterTransformation = beforeTransform.transpose() * transMatrix;
	res << afterTransformation.x(), afterTransformation.y(), afterTransformation.z();
	return res;
}

Vector3d Vertex::getScreenPosition(shared_ptr<Camera> camera, bool isPerspective, Vector2d screenCenter) {
	Vector4d beforeTransform;
	beforeTransform << position, 1;

	// TODO: инициализировать ее исходя из трансформации камеры
	Matrix4d cameraTransMatrix {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 } };

	Matrix4d finalTransMatrix;
	if (isPerspective) {
		Matrix4d perspectiveTransMatrix {
		{ 1, 0, 0, 0},
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, -0.001 },
		{ 0, 0, 0, 1 } };

		// TODO: Найти центр исходя из размера окна
		Vector3d center(screenCenter.x(), screenCenter.y(), 0);

		finalTransMatrix = transMatrix * cameraTransMatrix
			* moveMatrix(-center) * perspectiveTransMatrix * moveMatrix(center);
	} else {
		finalTransMatrix = transMatrix * cameraTransMatrix;
	}

	Vector4d afterTransformation = beforeTransform.transpose() * finalTransMatrix;
	Vector3d res;
	res << afterTransformation.x() / afterTransformation.w(), 
			afterTransformation.y() / afterTransformation.w(),
			afterTransformation.z() / afterTransformation.w();

	return res;
}

void Vertex::move(const Vector3d dif)
{
	transMatrix *= moveMatrix(dif);
}

void Vertex::scale(const Vector3d k)
{
	transMatrix *= scaleMatrix(k);
}

void Vertex::rotate(const Vector3d angles)
{
	rotateX(angles.x());
	rotateY(angles.y());
	rotateZ(angles.z());
}

void Vertex::rotateX(const double angle)
{
	transMatrix *= rotateXMatrix(angle);
}

void Vertex::rotateY(const double angle)
{
	transMatrix *= rotateYMatrix(angle);
}

void Vertex::rotateZ(const double angle)
{
	transMatrix *= rotateZMatrix(angle);
}



double getDistance2D(Vector2d v1, Vector2d v2)
{
	return sqrt((v1.x() - v2.x()) * (v1.x() - v2.x()) + (v1.y() - v2.y()) * (v1.y() - v2.y()));
}

Matrix4d moveMatrix(const Vector3d dif) {
	Matrix4d moveMatrix {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ dif.x(), dif.y(), dif.z(), 1 } };

	return moveMatrix;
}

Matrix4d scaleMatrix(const Vector3d k) {
	Matrix4d scaleMatrix {
		{ k.x(), 0,     0,     0 },
		{ 0,     k.y(), 0,     0 },
		{ 0,     0,     k.z(), 0 },
		{ 0,     0,     0,     1 } };

	return scaleMatrix;
}

Matrix4d rotateXMatrix(const double angle) {
	Matrix4d rotateMatrix {
		{ 1, 0,          0,           0 },
		{ 0, cos(angle), -sin(angle), 0 },
		{ 0, sin(angle),  cos(angle), 0 },
		{ 0, 0,           0,          1 } };

	return rotateMatrix;
}

Matrix4d rotateYMatrix(const double angle) {
	Matrix4d rotateMatrix {
		{ cos(angle),  0, sin(angle), 0 },
		{ 0,           1, 0,          0 },
		{ -sin(angle), 0, cos(angle), 0 },
		{ 0,           0, 0,          1 } };

	return rotateMatrix;
}

Matrix4d rotateZMatrix(const double angle) {
	Matrix4d rotateMatrix {
		{ cos(angle), -sin(angle), 0, 0 },
		{ sin(angle),  cos(angle), 0, 0 },
		{ 0,           0,          1, 0 },
		{ 0,           0,          0, 1 } };

	return rotateMatrix;
}