#pragma once
#include "../Object.h"
#include "../Model/PolygonalModelDetails/Vertex.h"
using namespace Eigen;
using namespace std;

class Camera : public Object
{
public:
	Camera();
	Camera(Vector3d pos);

	Vector3d getCenter();
	Matrix4d getTransMatrix();

	void transform(Vector3d move_params, Vector3d rotate_params);

private:
	void move(const Vector3d move_params);
	void rotate(const Vector3d rotate_params);

	Vertex pos;

	// Центр вращения камеры, к которому направлен ее объектив
	Vector3d center;
};

