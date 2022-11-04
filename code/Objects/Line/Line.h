#pragma once
#include "../Object.h"
#include "../Model/PolygonalModelDetails/Vertex.h"
using namespace Eigen;
using namespace std;

class Line : public Object
{
public:
	Line();
	Line(Vertex p1, Vertex p2);
	
	Vertex p1;
	Vertex p2;
};

