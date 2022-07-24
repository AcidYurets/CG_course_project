#pragma once
#include <string>
#include <vector>
#include <memory>
#include <Eigen/Dense>
using namespace Eigen;

class Vertex
{
public:
	Vertex();
	explicit Vertex(Vector3d pos);

	Vector3d getPosition();
	void setPosition(Vector3d pos);

	Vector2d getScreenPosition();

	void move(const Vector3d dif);
	void scale(const Vector3d k);
	void rotate(const Vector3d angles);
	
private:
	void rotateX(const double angle);
	void rotateY(const double angle);
	void rotateZ(const double angle);

	Vector3d position;
	Matrix4d transMatrix;
};
