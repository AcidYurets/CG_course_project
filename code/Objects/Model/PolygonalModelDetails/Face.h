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

	void setColor(uint color);

	bool selected = false;

private:
	Edges edges;
	Vertices vertices;
	uint color;
};
