#include "Camera.h"

Camera::Camera() : center(0, 0, 0) {
	this->pos.setPosition(Vector3d(0, 0, 0));
}

Camera::Camera(Vector3d pos) : center(0, 0, 0) {
	this->pos.setPosition(pos);
}

Vector3d Camera::getCenter() {
	return center;
}

Matrix4d Camera::getTransMatrix() {
	return pos.getTransMatrix();
}

void Camera::transform(Vector3d move_params, Vector3d rotate_params) {
	this->move(-center);

	this->rotate(rotate_params);

	center += move_params;
	this->move(center);
}

void Camera::move(const Vector3d move_params) {
	pos.move(move_params);
}

void Camera::rotate(const Vector3d rotate_params) {
	pos.rotate(rotate_params);
}
