#include "Vertex.h"

Vertex::Vertex() : position(0, 0, 0), transMatrix(Matrix4d::Identity()) { }

Vertex::Vertex(Vector3d pos) : Vertex()
{
	position = pos;
}

Vector3d Vertex::getPosition()
{
	return position;
}

void Vertex::setPosition(Vector3d pos)
{
	position = pos;
}

Vector2d Vertex::getScreenPosition()
{
	/*
	* TODO: Тут будет магия!
	*/
	Vector2d res = Vector2d(position.x(), position.y());
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


