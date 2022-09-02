#include "Vertex.h"
#include "Objects/Camera/Camera.h"

Vertex::Vertex() : position(0, 0, 0), transMatrix(Matrix4d::Identity()) { }

Vertex::Vertex(Vector3d pos) : Vertex() {
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
	/*
	* TODO: Тут будет магия!
	*/
	Vector3d res = Vector3d(transformPosition.x(), transformPosition.y(), transformPosition.z());
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
