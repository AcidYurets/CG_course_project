#pragma once
#include "Vertex.h"
using namespace Eigen;

class Edge
{
public:
	Edge();
	explicit Edge(Vector2<std::shared_ptr<Vertex>> vertices);

	void transform(Vector3d move_params, Vector3d scale_params, Vector3d rotate_params);

private:
	Vector2<std::shared_ptr<Vertex>> vertices;
	Vector3d center;
};
