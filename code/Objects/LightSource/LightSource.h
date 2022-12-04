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

	Vertex getPosition();
	string getName();

	void transform(Vector3d move_params, Vector3d scale_params, Vector3d rotate_params);

	int k = 300;
	bool selected = false;

private:
	Vertex pos;
	string name;
};

