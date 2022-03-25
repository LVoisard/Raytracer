#include "Light.h"
#include "PointLight.h"
#include <Eigen/Core>
#include "../external/json.hpp"
#include "Geometry.h"
#include <algorithm>
using namespace nlohmann;
using namespace Eigen;

Vector3f ParseVector3f(json j);
float Distance(Vector3f v1, Vector3f v2);

PointLight::PointLight(json j) : Light(j)
{
	centre = ParseVector3f(j["centre"]);
}

Vector3f PointLight::illuminate(Ray ray, HitRecord hrec)
{
	Vector3f hitPoint = ray.evaluate(hrec.t);
	Vector3f lightDir = (centre - hitPoint).normalized();
	Vector3f viewingRay = -(ray.evaluate(hrec.t).normalized());

	Vector3f color = hrec.hit->material.evaluate(lightDir, viewingRay, hrec.n, id, is);
	return color;
}

bool PointLight::IsShaded(Ray ray, std::vector<Geometry*> objects, HitRecord closestHit)
{
	bool isShaded = false;
	// check if the hit point is shadowed then color
			// start from the hit point, trace shadow ray towards the light,
			// look for intersection
	Vector3f startPoint = ray.evaluate(closestHit.t);
	Vector3f direction = (centre - startPoint).normalized();
	Ray shadowRay = Ray(startPoint, direction);
	Ray cameraCheckRay = Ray(startPoint, -ray.direction);
	for each (Geometry * g in objects)
	{
		if (g == closestHit.hit)
			continue;
		HitRecord cameraCheckRecord = g->hit(cameraCheckRay);
		if (cameraCheckRecord.t != Infinity)
			continue;
		HitRecord rec = g->hit(shadowRay);

		// hit is in a shadow
		if (rec.t > 0 && Distance(shadowRay.origin, shadowRay.evaluate(rec.t)) < Distance(shadowRay.origin, centre))
		{
			isShaded = true;
			break;
		}
	}
	return isShaded;
}
