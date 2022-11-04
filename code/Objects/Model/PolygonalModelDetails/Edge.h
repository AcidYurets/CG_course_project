#pragma once
#include "Vertex.h"
using namespace Eigen;
using namespace std;


class Edge
{
public:
	Edge();
	explicit Edge(Vector2<shared_ptr<Vertex>> vertices);
	Edge(shared_ptr<Vertex> v1, shared_ptr<Vertex> v2);

	Vector2<shared_ptr<Vertex>> getVertices();

	bool selected = false;

private:
	Vector2<shared_ptr<Vertex>> vertices;
};
