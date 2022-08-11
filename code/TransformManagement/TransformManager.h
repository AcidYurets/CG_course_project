#pragma once
#include "Scene/Scene.h"

class TransformManager
{
public:
	void transformModel(shared_ptr<Model>, 
		Vector3d move_params, Vector3d scale_params, Vector3d rotate_params);
};

