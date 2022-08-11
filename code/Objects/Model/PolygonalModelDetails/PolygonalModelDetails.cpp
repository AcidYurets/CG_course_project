#include "PolygonalModelDetails.h"
#include "Exceptions/Exceptions.h"

PolygonalModelDetails::PolygonalModelDetails() : vertices(0), edges(0), faces(0), center(0, 0, 0) { }

Vertices PolygonalModelDetails::getVertices() {
	return vertices;
}

Edges PolygonalModelDetails::getEdges() {
	return edges;
}

Faces PolygonalModelDetails::getFaces() {
	return faces;
}

Vector3d PolygonalModelDetails::getCenter() {
	return center;
}

void PolygonalModelDetails::setEdges(Edges edges) {
	this->edges = edges;
}

void PolygonalModelDetails::addVertex(std::shared_ptr<Vertex> vert) {
	vertices.push_back(vert);
}

void PolygonalModelDetails::addEdge(std::shared_ptr<Edge> edge) {
	edges.push_back(edge);
}

void PolygonalModelDetails::addFace(std::shared_ptr<Face> face) {
	faces.push_back(face);
}

void PolygonalModelDetails::setArithmeticCenter() {
	this->center = findArithmeticCenter();
}

void PolygonalModelDetails::transform(Vector3d move_params, Vector3d scale_params, Vector3d rotate_params) {
	this->move(-center);

	this->scale(scale_params);
	this->rotate(rotate_params);

	center += move_params;
	this->move(center);
}

Vector3d PolygonalModelDetails::findArithmeticCenter() {
	if (vertices.size() == 0)
	{
		throw EmptyException(EXCEPCION_ARGS, "Vertices is empty");
	}
	auto point = vertices[0]->getPosition();
	double maxX = point.x(), maxY = point.y(), maxZ = point.z();
	double minX = point.x(), minY = point.y(), minZ = point.z();

	for (auto& vert : vertices)
	{
		point = vert->getPosition();
		if (point.x() > maxX) maxX = point.x();
		if (point.y() > maxY) maxY = point.y();
		if (point.z() > maxZ) maxZ = point.z();

		if (point.x() < minX) minX = point.x();
		if (point.y() < minY) minY = point.y();
		if (point.z() < minZ) minZ = point.z();
	}

	return Vector3d((maxX + minX) / 2, (maxY + minY) / 2, (maxZ + minZ) / 2);
}

void PolygonalModelDetails::move(const Vector3d move_params) {
	for (auto& v : vertices)
		v->move(move_params);
}

void PolygonalModelDetails::scale(const Vector3d scale_params) {
	for (auto& v : vertices)
		v->scale(scale_params);
}

void PolygonalModelDetails::rotate(const Vector3d rotate_params) {
	for (auto& v : vertices)
		v->rotate(rotate_params);
}


