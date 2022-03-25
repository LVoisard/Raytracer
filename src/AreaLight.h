#pragma once
#include "Light.h"
#include <Eigen/Core>
#include "../external/json.hpp"
#include "Geometry.h"
#include <vector>

using namespace nlohmann;
using namespace Eigen;

class AreaLight : public Light
{
public: 
	AreaLight(json j);
	Vector3f p1;
	Vector3f p2;
	Vector3f p3;
	Vector3f p4;
	int n;
	bool useCenter;
	Vector3f centre;

	virtual Vector3f illuminate(Ray ray, HitRecord hrec);
	virtual bool IsShaded(Ray ray, std::vector<Geometry*> objects, HitRecord closestHit);
};