#pragma once
#include <Eigen/Core>
#include <Eigen/Dense>
#include "../external/json.hpp"
#include "Ray.h"
#include "HitRecord.h"
#include "Material.h"
#include <string>

using namespace nlohmann;
using namespace Eigen;

class HitRecord;

class Geometry
{
public:
	Geometry();
	Geometry(json j);
	virtual Vector3f GetIntersectionPoint(Ray r, float t);
	virtual HitRecord hit(Ray);

	bool visible;
	std::string comment;
	Material material;
	Matrix<float, 4, 4> transform;
};

