#include "Face.h"
#include "../../../Exceptions/Exceptions.h"

Face::Face() : edges(0), vertices(0) { }

Face::Face(Edges edges, Vertices vertices) : edges(edges), vertices(vertices) {
	if (vertices.size() == 0) throw EmptyException(EXCEPCION_ARGS, "Vertices is empty");
}

Vertices Face::getVertices() {
	return vertices;
}

uint Face::getColor() {
	return color;
}

Vector3d Face::getNormal(const shared_ptr<Camera>& camera, bool isPerspective, Vector2d screenCenter) {
	auto u = vertices[1]->getScreenPosition(camera, isPerspective, screenCenter) - vertices[0]->getScreenPosition(camera, isPerspective, screenCenter);
	auto v = vertices[2]->getScreenPosition(camera, isPerspective, screenCenter) - vertices[0]->getScreenPosition(camera, isPerspective, screenCenter);
	auto normal = u.cross(v);
	normal.normalize();
	return normal;
}

void Face::setColor(uint color) {
	this->color = color;
}
