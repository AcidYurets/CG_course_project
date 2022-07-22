#pragma once
#include "../Object.h"

class Model : public Object 
{
	virtual void transform(const Matrix<double> matrix) = 0;
};

