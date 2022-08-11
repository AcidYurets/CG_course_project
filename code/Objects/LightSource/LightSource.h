#pragma once
#include "../Object.h"
#include "../Model/PolygonalModelDetails/Vertex.h"
using namespace Eigen;
using namespace std;

class LightSource : public Object
{
public:
	LightSource();
	LightSource(Vertex v, string name);

private:
	Vertex pos;
	string name;
};

