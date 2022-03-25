#include "Geometry.h"
#include "Material.h"
#include <Eigen/Core>
#include <Eigen/Dense>
#include "../external/json.hpp"
#include <iostream>
using namespace std;


Vector3f ParseVector3f(json j);
Matrix4f ParseTransform(json);


Geometry::Geometry()
{
}

Geometry::Geometry(json j)
{
	material = Material(j);
	if (!j["transform"].is_null())
	{
		// transform
		transform = ParseTransform(j["transform"]);
	}
	else
	{
		transform = Matrix4f::Identity();
	}

	if (!j["visible"].is_null())
	{
		visible = j["visible"];
	}
	else
	{
		visible = true;
	}

	if (!j["comment"].is_null())
	{
		comment = j["comment"];
	}
}

HitRecord Geometry::hit(Ray ray)
{
	return HitRecord(this, Infinity, Vector3f());
}

/// <summary>
/// returns the 3D point where the ray hit the goemetry
/// </summary>
/// <param name="r"> Intersecting Ray </param>
/// <param name="t"> Distance </param>
/// <returns></returns>
Vector3f Geometry::GetIntersectionPoint(Ray r, float t)
{
	return Vector3f();
}