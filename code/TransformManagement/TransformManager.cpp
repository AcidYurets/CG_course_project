#include "TransformManager.h"

void TransformManager::transformModel(shared_ptr<Model> model, 
	Vector3d move_params, Vector3d scale_params, Vector3d rotate_params) {
	model->transform(move_params, scale_params, rotate_params);
}
