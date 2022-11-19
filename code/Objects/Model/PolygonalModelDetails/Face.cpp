#include "Face.h"
#include "../../../Exceptions/Exceptions.h"
#include <QRgb>

Face::Face() : edges(0), vertices(0), color(qRgb(200, 200, 200)) { }

Face::Face(Edges edges, Vertices vertices) : Face() {
	if (vertices.size() == 0) throw EmptyException(EXCEPCION_ARGS, "Vertices is empty");

	this->edges = edges;
	this->vertices = vertices;
}

Vertices Face::getVertices() {
	return vertices;
}

Edges Face::getEdges() {
	return edges;
}


uint Face::getColor() {
	return color;
}

Vector3d Face::getNormal(const shared_ptr<Camera>& camera, bool isPerspective, Vector2d screenCenter) {
	if (vertices.size() < 3) throw EmptyException(EXCEPCION_ARGS, "CRITICAL: Face has less then 3 vertices!");

	auto u = vertices[1]->getScreenPosition(camera, isPerspective, screenCenter) - vertices[0]->getScreenPosition(camera, isPerspective, screenCenter);
	auto v = vertices[2]->getScreenPosition(camera, isPerspective, screenCenter) - vertices[0]->getScreenPosition(camera, isPerspective, screenCenter);
	auto normal = u.cross(v);
	normal.normalize();
	return normal;
}

Vertex Face::getCenter() {
	Vertex v;
	v.setPosition(findArithmeticCenter());
	return v;
}

void Face::setColor(uint color) {
	this->color = color;
}

void Face::transform(Vector3d move_params, Vector3d scale_params, Vector3d rotate_params) {
	Vector3d center = findArithmeticCenter();

	this->move(-center);

	this->scale(scale_params);
	this->rotate(rotate_params);

	center += move_params;
	this->move(center);
}

Vector3d Face::findArithmeticCenter() {
	if (vertices.size() == 0)
	{
		throw EmptyException(EXCEPCION_ARGS, "Vertices is empty");
	}
	auto point = vertices[0]->getTransformPosition();
	double maxX = point.x(), maxY = point.y(), maxZ = point.z();
	double minX = point.x(), minY = point.y(), minZ = point.z();

	for (auto& vert : vertices)
	{
		point = vert->getTransformPosition();
		if (point.x() > maxX) maxX = point.x();
		if (point.y() > maxY) maxY = point.y();
		if (point.z() > maxZ) maxZ = point.z();

		if (point.x() < minX) minX = point.x();
		if (point.y() < minY) minY = point.y();
		if (point.z() < minZ) minZ = point.z();
	}

	return Vector3d((maxX + minX) / 2, (maxY + minY) / 2, (maxZ + minZ) / 2);
}

void Face::move(const Vector3d move_params) {
	for (auto& v : vertices)
		v->move(move_params);
}

void Face::scale(const Vector3d scale_params) {
	for (auto& v : vertices)
		v->scale(scale_params);
}

void Face::rotate(const Vector3d rotate_params) {
	for (auto& v : vertices)
		v->rotate(rotate_params);
}
