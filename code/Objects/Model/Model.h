#pragma once
#include "../Object.h"
#include "PolygonalModelDetails/PolygonalModelDetails.h"

class Model : public Object 
{
public:
	Model();
	Model(shared_ptr<PolygonalModelDetails> details, string name);

	string getName();
	shared_ptr<PolygonalModelDetails> getDetails();

	virtual void transform(Vector3d move_params, Vector3d scale_params, Vector3d rotate_params);

	bool selected = false;

private:
	shared_ptr<PolygonalModelDetails> details;
	string name;
};

