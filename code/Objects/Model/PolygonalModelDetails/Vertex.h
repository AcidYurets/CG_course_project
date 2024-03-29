#pragma once
#include <string>
#include <vector>
#include <memory>
#include <Eigen/Dense>
#include <QDebug>
using namespace Eigen;
using namespace std;

class Camera;

class Vertex
{
public:
	Vertex();
	explicit Vertex(Vector3d pos);
	Vertex(double x, double y, double z);

	// Vector3d getPosition();
	void setPosition(Vector3d pos);

	Matrix4d getTransMatrix();

	// Метод находит реальное местоположение вершины без учета местоположения камеры
	Vector3d getTransformPosition();

	// Метод находит местоположение вершины на экране с учетом камеры 
	// и параметров отображения (например, перспективы)
	Vector3d getScreenPosition(shared_ptr<Camera> camera, bool isPerspective, Vector2d screenCenter);

	void move(const Vector3d move_params);
	void scale(const Vector3d scale_params);
	void rotate(const Vector3d rotate_params);

	void transform(Vector3d move_params, Vector3d scale_params, Vector3d rotate_params);

	bool selected = false;
	
private:
	void rotateX(const double angle);
	void rotateY(const double angle);
	void rotateZ(const double angle);

	Vector3d position;
	Matrix4d transMatrix;
};

double getDistance2D(Vector2d v1, Vector2d v2);
double getDistToLine2D(Vector2d p, Vector2d p1, Vector2d p2);

Matrix4d moveMatrix(const Vector3d dif);
Matrix4d scaleMatrix(const Vector3d k);
Matrix4d rotateXMatrix(const double angle);
Matrix4d rotateYMatrix(const double angle);
Matrix4d rotateZMatrix(const double angle);
