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
	
private:
	Edges edges;
	Vertices vertices;
};
