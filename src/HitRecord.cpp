#include "HitRecord.h"
#include "Geometry.h"
#include <Eigen/Core>

using namespace Eigen;

class Geometry;

HitRecord::HitRecord()
{
	this->hit = nullptr;
	this->t = Infinity;
	this->n = Vector3f(0,0,0);
}

HitRecord::HitRecord(Geometry* hit, float t, Vector3f n)
{
	this->hit = hit;
	this->t = t;
	this->n = n;
}
