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

void Face::setColor(uint color) {
	this->color = color;
}
