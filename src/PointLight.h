#pragma once
#include "Light.h"
#include <Eigen/Core>
#include "../external/json.hpp"
using namespace nlohmann;
using namespace Eigen;

class PointLight : public Light
{
public:
	PointLight(json j);
	Matrix<float, 3, 1> centre;
	virtual Vector3f illuminate(Ray ray, HitRecord hrec);
	virtual bool IsShaded(Ray ray, std::vector<Geometry*> objects, HitRecord closestHit);
};