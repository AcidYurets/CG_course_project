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

	Vertices getVertices();
	Edges getEdges();

	uint getColor();
	Vector3d getNormal(const shared_ptr<Camera>& camera, bool isPerspective, Vector2d screenCenter);
	Vertex getCenter();

	void setColor(uint color);

	void transform(Vector3d move_params, Vector3d scale_params, Vector3d rotate_params);

	bool selected = false;
	// Номер грани в модели
	int number = 0;

private:
	Vector3d findArithmeticCenter();
	void move(const Vector3d move_params);
	void scale(const Vector3d scale_params);
	void rotate(const Vector3d rotate_params);

	Edges edges;
	Vertices vertices;
	uint color;
};
