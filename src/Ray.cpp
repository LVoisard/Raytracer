#include "Ray.h"
#include <Eigen/Core>
#include <Eigen/Dense>

using namespace Eigen;

Ray::Ray()
{
}

Ray::Ray(Vector3f origin, Vector3f direction)
{
	this->origin = origin;
	this->direction = direction;
}

Vector3f Ray::evaluate(float t)
{
	return origin + t * direction;
}
