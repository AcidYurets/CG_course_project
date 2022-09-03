#include "Vertex.h"
#include "Objects/Camera/Camera.h"

Vertex::Vertex() : position(0, 0, 0), transMatrix(Matrix4d::Identity()) {
	// this->move(Vector3d(370, 280, 0));
	// this->scale(Vector3d(100, 100, 100));
}

Vertex::Vertex(Vector3d pos) : Vertex() {
	// pos *= 70;
	// position = Vector3d(pos.x() + 370, pos.y() + 280, pos.z());
	position = pos;
}

Vertex::Vertex(double x, double y, double z) : Vertex(Vector3d(x, y, z)) { }

Vector3d Vertex::getPosition() {
	return position;
}

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

Vector3d Vertex::getScreenPosition(shared_ptr<Camera> camera, bool perspective) {
	Vector3d transformPosition = getTransformPosition();
	// transformPosition *= 100;

	Vector4d beforeCameraTransform;
	beforeCameraTransform << transformPosition, 1;

	// TODO: инициализировать ее исходя из трансформации камеры
	Matrix4d cameraTransMatrix {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 } };
	// cameraTransMatrix(2, 2) = 100;
	// cameraTransMatrix(2, 3) = -0.01;

	Matrix4d perspectiveTransMatrix {
		{ 1, 0, 0, 0},
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0.1 },
		{ 0, 0, 0, 1 } };

	cameraTransMatrix *= perspectiveTransMatrix;
	

	Vector4d afterTransformation = beforeCameraTransform.transpose() * cameraTransMatrix;
	Vector3d res;
	res << afterTransformation.x() / afterTransformation.w(), 
			afterTransformation.y() / afterTransformation.w(),
			afterTransformation.z() / afterTransformation.w();
	// Vector3d res = Vector3d(transformPosition.x(), transformPosition.y(), transformPosition.z());

	// Масштабируем и переносим в центр для корректного отображения
	res *= 100;
	res = Vector3d(res.x() + 370, res.y() + 280, res.z());

	return res;
}

void Vertex::move(const Vector3d dif)
{
	Matrix4d moveMatrix {
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ dif.x(), dif.y(), dif.z(), 1 } };

	transMatrix *= moveMatrix;
}

void Vertex::scale(const Vector3d k)
{
	Matrix4d scaleMatrix {
		{ k.x(), 0,     0,     0 },
		{ 0,     k.y(), 0,     0 },
		{ 0,     0,     k.z(), 0 },
		{ 0,     0,     0,     1 } };

	transMatrix *= scaleMatrix;
}

void Vertex::rotate(const Vector3d angles)
{
	rotateX(angles.x());
	rotateY(angles.y());
	rotateZ(angles.z());
}

void Vertex::rotateX(const double angle)
{
	Matrix4d rotateMatrix {
		{ 1, 0,          0,           0 },
		{ 0, cos(angle), -sin(angle), 0 },
		{ 0, sin(angle),  cos(angle), 0 },
		{ 0, 0,           0,          1 } };

	transMatrix *= rotateMatrix;
}

void Vertex::rotateY(const double angle)
{
	Matrix4d rotateMatrix {
		{ cos(angle),  0, sin(angle), 0 },
		{ 0,           1, 0,          0 },
		{ -sin(angle), 0, cos(angle), 0 },
		{ 0,           0, 0,          1 } };

	transMatrix *= rotateMatrix;
}

void Vertex::rotateZ(const double angle)
{
	Matrix4d rotateMatrix{
		{ cos(angle), -sin(angle), 0, 0 },
		{ sin(angle),  cos(angle), 0, 0 },
		{ 0,           0,          1, 0 },
		{ 0,           0,          0, 1 } };

	transMatrix *= rotateMatrix;
}



double getDistance2D(Vector2d v1, Vector2d v2)
{
	return sqrt((v1.x() - v2.x()) * (v1.x() - v2.x()) + (v1.y() - v2.y()) * (v1.y() - v2.y()));
}
