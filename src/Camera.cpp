#define _USE_MATH_DEFINES
#include "Camera.h"
#include <Eigen/Core>
#include "../external/json.hpp"

using nlohmann::json;
using namespace Eigen;

Vector3f ParseVector3f(json j);

Camera::Camera()
{
}

Camera::Camera(json j)
{	
	up = ParseVector3f(j["up"]);
	fov = j["fov"];
	centre = ParseVector3f(j["centre"]);
	lookat = ParseVector3f(j["lookat"]);
	right = lookat.cross(up).normalized();
	pixelSize = (2 * tanf(fov / 2 * M_PI / 180)) / j["size"][1];
	A = lookat;
	B = (A + tanf(fov / 2 * M_PI / 180) * up);
	C = B - (j["size"][0] / 2 * pixelSize * right);
	
}

Ray Camera::CalculateViewingRay(float x, float y, bool antialiasing)
{
	Vector3f P;
	if (antialiasing)
	{
		P = C + ((x * pixelSize) * right) - ((y * pixelSize) * up);
	}
	else
	{
		P = C + ((x * pixelSize + pixelSize / 2) * right) - ((y * pixelSize + pixelSize / 2) * up);

	}
	return Ray(centre, P.normalized());
}
