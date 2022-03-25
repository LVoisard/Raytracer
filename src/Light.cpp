#include "Light.h"
#include "HitRecord.h"
#include "Material.h"
#include "Geometry.h"
#include <Eigen/Core>
#include "../external/json.hpp"

using namespace nlohmann;
using namespace Eigen;

Vector3f ParseVector3f(json j);
Matrix4f ParseTransform(json j);

Light::Light(json j)
{
	id = ParseVector3f(j["id"]);
	is = ParseVector3f(j["is"]);

	if (!j["transform"].is_null())
	{
		transform = ParseTransform(j["transform"]);
	}


	if (!j["use"].is_null())
	{
		use = j["use"].get<bool>();
	}
	else
	{
		use = true;
	}
}

Vector3f Light::illuminate(Ray ray, HitRecord hrec)
{
	return Vector3f(0, 0, 0);
}

bool Light::IsShaded(Ray ray, std::vector<Geometry*> objects, HitRecord closestHit)
{
	return false;
}
