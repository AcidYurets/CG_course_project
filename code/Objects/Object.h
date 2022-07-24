#pragma once
#include <Eigen/Dense>
using namespace Eigen;

class Object
{
	virtual void transform(const Matrix4d matrix) = 0;
};


