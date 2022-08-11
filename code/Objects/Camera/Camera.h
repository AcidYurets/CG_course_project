#pragma once
#include "../Object.h"
#include "../Model/PolygonalModelDetails/Vertex.h"
using namespace Eigen;
using namespace std;

class Camera : public Object
{
public:
	Camera();
	Camera(Vertex pos);

private:
	Vertex pos;
	// Центр вращения камеры, к которому направлен ее объектив.
	Vector3d center;
};

