#pragma once
#include <Eigen/Core>
#include "../external/json.hpp"
#include "Ray.h"

using nlohmann::json;
using namespace Eigen;

class Camera
{
public:
	Camera();
	Camera(json);
	Ray CalculateViewingRay(float x, float y, bool antialiasing = false);
	Vector3f centre;
	Vector3f lookat;
private:
	Vector3f up;
	Vector3f right;
	Vector3f A;
	Vector3f B;
	Vector3f C;
	float fov;
	float pixelSize;
};