#pragma once
#include "Geometry.h"
#include "../external/json.hpp"
#include <Eigen/Core>
#include "Ray.h"
#include "HitRecord.h"


class Geometry;

class Rectangle : public Geometry
{
public:
	Rectangle(json j);
	virtual Vector3f GetIntersectionPoint(Ray r, float t);
	virtual HitRecord hit(Ray);


	Vector3f normal;
	Vector3f p1;
	Vector3f p2;
	Vector3f p3;
	Vector3f p4;
};