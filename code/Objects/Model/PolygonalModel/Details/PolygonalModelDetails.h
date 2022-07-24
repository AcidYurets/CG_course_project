#pragma once
#include <vector>
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
using namespace std;

using Vertices = std::vector<std::shared_ptr<Vertex>>;
using Edges = std::vector<std::shared_ptr<Edge>>;
using Faces = std::vector<std::shared_ptr<Face>>;

class PolygonalModelDetails
{
public:
	PolygonalModelDetails();

	void addVertex(std::shared_ptr<Vertex> vert);
	void addEdge(std::shared_ptr<Edge> edge);
	void addFace(std::shared_ptr<Face> face);
	void setArithmeticCenter();


	void setCenter(Vector3d center);

	void transform(Vector3d move_params, Vector3d scale_params, Vector3d rotate_params);

private:
	Vector3d findArithmeticCenter();

	Vertices vertices;
	Edges edges;
	Faces faces;
	Vector3d center;

	string name;
};

