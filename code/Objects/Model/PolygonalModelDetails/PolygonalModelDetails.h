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

	Vertices getVertices();
	Edges getEdges();
	Faces getFaces();	
	Vertex getCenter();
	
	void addVertex(std::shared_ptr<Vertex> vert);
	void addEdge(std::shared_ptr<Edge> edge);
	void addFace(std::shared_ptr<Face> face);

	void setEdges(Edges edges);

	void setArithmeticCenter();
	void setCenter(Vector3d center);

	void transform(Vector3d move_params, Vector3d scale_params, Vector3d rotate_params);

private:
	Vector3d findArithmeticCenter();
	void move(const Vector3d move_params);
	void scale(const Vector3d scale_params);
	void rotate(const Vector3d rotate_params);

	Vertices vertices;
	Edges edges;
	Faces faces;

	Vertex center;
};

