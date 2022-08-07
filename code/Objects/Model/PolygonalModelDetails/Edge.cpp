#include "Edge.h"

Edge::Edge() : vertices(nullptr, nullptr){ }

Edge::Edge(Vector2<shared_ptr<Vertex>> vertices) : vertices(vertices) { }

Edge::Edge(shared_ptr<Vertex> v1, shared_ptr<Vertex> v2) : Edge(Vector2<shared_ptr<Vertex>>(v1, v2)) { }

Vector2<shared_ptr<Vertex>> Edge::getVertices() {
	return vertices;
}

void Edge::transform(Vector3d move_params, Vector3d scale_params, Vector3d rotate_params) {
    Vector3d center = findArithmeticCenter();
    this->move(-center);

    this->scale(scale_params);
    this->rotate(rotate_params);

    // Сдвиг центра на move_params
    center += move_params;
    this->move(center);
}

Vector3d Edge::findArithmeticCenter() {
    auto point1 = vertices.x()->getPosition();
    auto point2 = vertices.y()->getPosition();

    return Vector3d((point1.x() + point2.x()) / 2, (point1.y() + point2.y()) / 2, (point1.z() + point2.z()) / 2);
}

void Edge::move(const Vector3d move_params) {
    vertices.x()->move(move_params);
    vertices.y()->move(move_params);
}

void Edge::scale(const Vector3d scale_params) {
    vertices.x()->scale(scale_params);
    vertices.y()->scale(scale_params);
}

void Edge::rotate(const Vector3d rotate_params) {
    vertices.x()->rotate(rotate_params);
    vertices.y()->rotate(rotate_params);
}

