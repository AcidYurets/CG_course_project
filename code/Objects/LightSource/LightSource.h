#pragma once
#include "../Object.h"

class LightSource : public Object
{
	virtual void transform(const Matrix4d matrix);
};

