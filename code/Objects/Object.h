#pragma once
#include <Math/Matrix.h>
using namespace std;

class Object
{
	virtual void transform(const Matrix<double> matrix) = 0;
};

