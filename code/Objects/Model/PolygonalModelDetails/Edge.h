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

	void transform(Vector3d move_params, Vector3d scale_params, Vector3d rotate_params);

private:
	Vector3d findArithmeticCenter();
	void move(const Vector3d move_params);
	void scale(const Vector3d scale_params);
	void rotate(const Vector3d rotate_params);

	Vector2<shared_ptr<Vertex>> vertices;
};
