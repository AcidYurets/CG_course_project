#include "TransformManager.h"

void TransformManager::transformModel(shared_ptr<Model> model, 
	Vector3d move_params, Vector3d scale_params, Vector3d rotate_params) {
	model->transform(move_params, scale_params, rotate_params);
}

void TransformManager::transformFace(shared_ptr<Face> face, 
	Vector3d move_params, Vector3d scale_params, Vector3d rotate_params) {
	face->transform(move_params, scale_params, rotate_params);
}

void TransformManager::transformEdge(shared_ptr<Edge> edge, 
	Vector3d move_params, Vector3d scale_params, Vector3d rotate_params) {
	edge->transform(move_params, scale_params, rotate_params);
}

void TransformManager::transformVertex(shared_ptr<Vertex> vertex,
	Vector3d move_params, Vector3d scale_params, Vector3d rotate_params) {
	vertex->transform(move_params, scale_params, rotate_params);
}

void TransformManager::transformCamera(shared_ptr<Camera> camera,
	Vector3d move_params, Vector3d rotate_params) {
	camera->transform(move_params, rotate_params);
}
