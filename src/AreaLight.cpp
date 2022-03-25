#include "AreaLight.h"
#include "Light.h"
#include <Eigen/Core>
#include "../external/json.hpp"
#include "Geometry.h"
#include <vector>
using namespace nlohmann;
using namespace Eigen;

Vector3f ParseVector3f(json);
float Distance(Vector3f v1, Vector3f v2);

AreaLight::AreaLight(json j): Light(j)
{
	p1 = ParseVector3f(j["p1"]);
	p2 = ParseVector3f(j["p2"]);
	p3 = ParseVector3f(j["p3"]);
	p4 = ParseVector3f(j["p4"]);

	if (!j["n"].is_null())
	{
		n = j["n"].get<int>();
	}
	else
	{
		n = 1;
	}

	if (!j["usecenter"].is_null())
	{
		useCenter = j["usecenter"].get<bool>();
		centre = Vector3f((p3.x() + p1.x()) / 2,
			(p1.y() + p3.y()) / 2,
			(p1.z() + p3.z()) / 2);
	}
	else
	{
		useCenter = false;
	}
}

Vector3f AreaLight::illuminate(Ray ray, HitRecord hrec)
{
	int samples = n;
	Vector3f color = Vector3f(0, 0, 0);
	Vector3f hitPoint = ray.evaluate(hrec.t);
	Vector3f viewingRay = -(ray.evaluate(hrec.t).normalized());

	if (useCenter)
	{
		Vector3f lightDir = (centre - hitPoint).normalized();
		color = hrec.hit->material.evaluate(lightDir, viewingRay, hrec.n, id, is);
		return color;
	}
	else
	{
		Vector3f diff = p3 - p1;

		for (int i = 0; i < samples; i++)
		{
			Vector3f samplePoint = Vector3f(p1.x() + diff.x() * ((float)rand() / RAND_MAX),
				p1.y() + diff.y() * (float)rand() / RAND_MAX,
				p1.z() + diff.z() * (float)rand() / RAND_MAX);
			Vector3f lightDir = (samplePoint - hitPoint).normalized();
			color += hrec.hit->material.evaluate(lightDir, viewingRay, hrec.n, id, is);
		}
		return color / samples;
	}
	

}

bool AreaLight::IsShaded(Ray ray, std::vector<Geometry*> objects, HitRecord closestHit)
{
	bool isShaded = false;
	Vector3f startPoint = ray.evaluate(closestHit.t);
	Vector3f diff = p3 - p1;
	if (!useCenter) {
		for (int j = 0; j < n; j++)
		{
			Vector3f samplePoint = Vector3f(p1.x() + diff.x() * ((float)rand() / RAND_MAX),
				p1.y() + diff.y() * (float)rand() / RAND_MAX,
				p1.z() + diff.z() * (float)rand() / RAND_MAX);
			
			Vector3f direction = (samplePoint - startPoint).normalized();
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
				if (rec.t > 0 && Distance(shadowRay.origin, shadowRay.evaluate(rec.t)) < Distance(shadowRay.origin, samplePoint))
				{
					isShaded = true;
					break;
				}
			}
		}
	}
	else
	{
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
	}
	return isShaded;
}
