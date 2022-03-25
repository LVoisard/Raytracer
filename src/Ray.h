#pragma once

#include <Eigen/Core>
#include <Eigen/Dense>

using namespace Eigen;

class Ray
{
public:
	Ray();
	Ray(Matrix<float, 3, 1> origin, Matrix<float, 3, 1> direction);
	Vector3f evaluate(float t);

	Matrix<float, 3, 1> origin;
	Matrix<float, 3, 1> direction;
};