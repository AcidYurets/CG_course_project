#pragma once
#include "../Object.h"
#include "PolygonalModelDetails/PolygonalModelDetails.h"

class Model : public Object 
{
public:
	Model();
	Model(shared_ptr<PolygonalModelDetails> details, string name);

	virtual void transform(const Matrix4d matrix);

private:
	shared_ptr<PolygonalModelDetails> details;

	string name;
};

