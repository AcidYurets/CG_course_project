#include "Edge.h"
#include "../../../Exceptions/Exceptions.h"

Edge::Edge() : vertices(nullptr, nullptr){ }

Edge::Edge(Vector2<shared_ptr<Vertex>> vertices) : vertices(vertices) { }

Edge::Edge(shared_ptr<Vertex> v1, shared_ptr<Vertex> v2) : Edge(Vector2<shared_ptr<Vertex>>(v1, v2)) { }

Vector2<shared_ptr<Vertex>> Edge::getVertices() {
	return vertices;
}

Vertex Edge::getCenter() {
	Vertex v;
	v.setPosition(findArithmeticCenter());
	return v;
}

void Edge::transform(Vector3d move_params, Vector3d scale_params, Vector3d rotate_params) {
	Vector3d center = findArithmeticCenter();

	this->move(-center);

	this->scale(scale_params);
	this->rotate(rotate_params);

	center += move_params;
	this->move(center);
}

Vector3d Edge::findArithmeticCenter() {
	if (vertices.size() == 0) {
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

void Edge::move(const Vector3d move_params) {
	for (auto& v : vertices)
		v->move(move_params);
}

void Edge::scale(const Vector3d scale_params) {
	for (auto& v : vertices)
		v->scale(scale_params);
}

void Edge::rotate(const Vector3d rotate_params) {
	for (auto& v : vertices)
		v->rotate(rotate_params);
}
