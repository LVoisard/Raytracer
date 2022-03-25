#pragma once
#include "Ray.h"
#include "HitRecord.h"
#include <Eigen/Core>
#include "../external/json.hpp"
using namespace nlohmann;
using namespace Eigen;


class Light
{
public:
	Light(json j);

	virtual Vector3f illuminate(Ray ray, HitRecord hrec);
	virtual bool IsShaded(Ray ray, std::vector<Geometry*> objects, HitRecord closestHit);

	//Diffuse light intensity
	Vector3f id;
	//Specular light intensity
	Vector3f is;

	Matrix<float, 4, 4> transform;

	bool use;
};