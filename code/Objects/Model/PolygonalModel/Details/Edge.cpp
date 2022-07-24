#include "Edge.h"

Edge::Edge() : vertices(nullptr, nullptr), center(0, 0, 0) { }

Edge::Edge(Vector2<std::shared_ptr<Vertex>> vertices) : vertices(vertices)
{
	auto point1 = vertices[0]->getPosition();
	auto point2 = vertices[0]->getPosition();

	center << (point1.x() + point2.x()) / 2, (point1.y() + point2.y()) / 2, (point1.z() + point2.z()) / 2;

}

