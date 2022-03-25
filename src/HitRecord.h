#pragma once
#include <Eigen/Core>

using namespace Eigen;

class Geometry;

class HitRecord
{
public:
	HitRecord();
	HitRecord(Geometry*, float, Vector3f);
	Geometry* hit;
	float t;
	Vector3f n;
};