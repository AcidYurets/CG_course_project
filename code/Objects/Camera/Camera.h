#pragma once
#include "../Object.h"

class Camera : public Object
{
	virtual void transform(const Matrix4d matrix);
};

