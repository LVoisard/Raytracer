#define _USE_MATH_DEFINES

#include <Eigen/Core>
#include <Eigen/Dense>
#include "../external/json.hpp"
#include <math.h>

using namespace Eigen;
using namespace nlohmann;


Vector3f ParseVector3f(json j)
{
	Vector3f newV;
	for (int i = 0; i < j.size(); i++)
	{
		newV(i) = j[i].get<float>();
	}
	return newV;
}

Matrix<unsigned int, 2, 1> ParseVector2uint(json j)
{
	Matrix<unsigned int, 2, 1> newM;
	for (int i = 0; i < j.size(); i++)
	{
		newM(i) = j[i].get<unsigned int>();
	}
	return newM;
}

Matrix4f ParseTransform(json j)
{
	Matrix4f newM;
	for (int i = 0; i < j.size(); i++)
	{
		newM((int)(i / 4), (int)i % 4) = j[i].get<float>();
	}
	return newM;
}

Vector3f CalculateRayDirection(Matrix<int, 2, 1> pixel, Vector3f origin, Vector3f up, Vector3f lookat, float fov, float height, float width)
{
	Vector3f realLookAt = (lookat - origin);
	float pixelSize = (2 * tanf(fov / 2 * M_PI / 180)) / height;
	Vector3f A = origin + realLookAt;
	Vector3f B = (A + tanf(fov / 2 * M_PI / 180) * up);
	Vector3f r = realLookAt.cross(up).normalized();
	Vector3f C = B - (width / 2 * pixelSize * r);
	Vector3f P = C + ((pixel.x() * pixelSize + pixelSize / 2) * r) - ((pixel.y() * pixelSize + pixelSize / 2) * up);
	return P.normalized();
}

float Distance(Vector3f v1, Vector3f v2)
{
	return sqrtf(powf(v2.x() - v1.x(), 2) + powf(v2.y() - v1.y(), 2) + powf(v2.z() - v1.z(), 2));
}