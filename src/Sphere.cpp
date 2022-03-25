#include "Sphere.h"
#include "Geometry.h"
#include <Eigen/Core>
#include <Eigen/Dense>

#include "../external/json.hpp"


Vector3f ParseVector3f(json);

Sphere::Sphere(json j) : Geometry(j)
{
	radius = j["radius"].get<float>();
	centre = ParseVector3f(j["centre"]);
}

Vector3f Sphere::GetIntersectionPoint(Ray r, float t)
{
	return r.origin + t * r.direction;
}

HitRecord Sphere::hit(Ray ray)
{
	float a = ray.direction.dot(ray.direction);
	float b = 2 * (ray.direction).dot(ray.origin - centre);
	float c = (ray.origin - centre).dot(ray.origin - centre) - radius * radius;

	float discriminant = b * b - (4 * a * c);
	float t = Infinity;

	if (discriminant > 0)
	{
		float t0 = (-b - sqrtf(b * b - 4 * a * c)) / (2 * a);
		float t1 = (-b + sqrtf(b * b - 4 * a * c)) / (2 * a);

		Vector3f v;
		if (t0 > t1)
		{
			t = t1;
		}
		else
		{
			t = t0;
		}
	}

	Vector3f normal = ((ray.evaluate(t)) - centre) / radius;

	if (normal.x() == -0)
		normal.x() = 0;
	if (normal.y() == -0)
		normal.y() = 0;
	if (normal.z() == -0)
		normal.z() = 0;

	return HitRecord(this, t, normal);
}
