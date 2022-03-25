#pragma once
#include "Geometry.h"
#include "../external/json.hpp"
#include <Eigen/Core>
#include "Ray.h"
#include "HitRecord.h"

using namespace nlohmann;

class Sphere: public Geometry
{
public:
	Sphere(json j);
	virtual Vector3f GetIntersectionPoint(Ray r, float t);
	virtual HitRecord hit(Ray);

	Matrix<float, 3, 1> centre;
	float radius;
};