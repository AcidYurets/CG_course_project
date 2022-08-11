#include "Model.h"

Model::Model() : details(nullptr), name("Nameless") { }

Model::Model(shared_ptr<PolygonalModelDetails> details, string name) : details(details), name(name) { }

string Model::getName() {
	return name;
}

shared_ptr<PolygonalModelDetails> Model::getDetails() {
	return details;
}

void Model::transform(Vector3d move_params, Vector3d scale_params, Vector3d rotate_params) {
	details->transform(move_params, scale_params, rotate_params);
}


