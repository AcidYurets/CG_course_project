#pragma once
#include "../Scene/Scene.h"

class TransformManager
{
public:
	void transformModel(shared_ptr<Model>, 
		Vector3d move_params, Vector3d scale_params, Vector3d rotate_params);
	void transformFace(shared_ptr<Face>,
		Vector3d move_params, Vector3d scale_params, Vector3d rotate_params);
	void transformEdge(shared_ptr<Edge>,
		Vector3d move_params, Vector3d scale_params, Vector3d rotate_params);
	void transformVertex(shared_ptr<Vertex>, 
		Vector3d move_params, Vector3d scale_params, Vector3d rotate_params);

	void transformCamera(shared_ptr<Camera> camera,
		Vector3d move_params, Vector3d rotate_params);
};

