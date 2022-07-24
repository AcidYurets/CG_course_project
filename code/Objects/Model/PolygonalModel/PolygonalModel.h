#pragma once
#include "../Model.h"
class PolygonalModel : public Model
{
	virtual void transform(const Matrix4d matrix);
};

