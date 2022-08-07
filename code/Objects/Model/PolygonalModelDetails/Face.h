#pragma once
#include "Edge.h"
using namespace Eigen;

using Vertices = std::vector<std::shared_ptr<Vertex>>;
using Edges = std::vector<std::shared_ptr<Edge>>;

class Face
{
public:
	Face();
	Face(Edges edges, Vertices vertices);

	void transform(Vector3d move_params, Vector3d scale_params, Vector3d rotate_params);
	
private:
	Vector3d findArithmeticCenter();
	void move(const Vector3d move_params);
	void scale(const Vector3d scale_params);
	void rotate(const Vector3d rotate_params);

	Edges edges;
	Vertices vertices;
};
