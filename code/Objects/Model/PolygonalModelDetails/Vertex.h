#pragma once
#include <string>
#include <vector>
#include <memory>
#include <Eigen/Dense>
#include <QDebug>
using namespace Eigen;

class Vertex
{
public:
	Vertex();
	explicit Vertex(Vector3d pos);
	Vertex(double x, double y, double z);

	Vector3d getPosition();
	void setPosition(Vector3d pos);

	Vector3d getTransformPosition();
	Vector2d getScreenPosition();

	void move(const Vector3d move_params);
	void scale(const Vector3d scale_params);
	void rotate(const Vector3d rotate_params);
	
private:
	void rotateX(const double angle);
	void rotateY(const double angle);
	void rotateZ(const double angle);

	Vector3d position;
	Matrix4d transMatrix;
};

double getDistance2D(Vector2d v1, Vector2d v2);
