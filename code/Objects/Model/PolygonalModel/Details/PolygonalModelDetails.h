#pragma once
#include <vector>
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
using namespace std;

class PolygonalModelDetails
{
public:
	using Vertices = vector<Vertex>;
	using Edges = vector<Edge>;
	using Faces = vector<Face>;

private:
	Vertices vertices;
	Edges edges;
	Faces faces;
};

