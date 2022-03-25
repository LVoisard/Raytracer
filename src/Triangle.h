#pragma once
#include <Eigen/Core>

using namespace Eigen;
class Triangle
{
public:
	Triangle();
	Triangle(Vector3f A, Vector3f B, Vector3f C);
	Vector3f A;
	Vector3f B;
	Vector3f C;
};