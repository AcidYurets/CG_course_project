#include "Edge.h"

Edge::Edge() : vertices(nullptr, nullptr){ }

Edge::Edge(Vector2<shared_ptr<Vertex>> vertices) : vertices(vertices) { }

Edge::Edge(shared_ptr<Vertex> v1, shared_ptr<Vertex> v2) : Edge(Vector2<shared_ptr<Vertex>>(v1, v2)) { }

Vector2<shared_ptr<Vertex>> Edge::getVertices() {
	return vertices;
}
