#include "Rectangle.h"
#include "Geometry.h"
#include "Triangle.h"
#include "../external/json.hpp"
#include <vector>
#include <Eigen/Core>
#include <Eigen/Dense>


using std::vector;
using namespace Eigen;

Vector3f ParseVector3f(json);
float Distance(Vector3f v1, Vector3f v2);

Rectangle::Rectangle(json j) : Geometry(j)
{
	p1 = ParseVector3f(j["p1"]);
	p2 = ParseVector3f(j["p2"]);
	p3 = ParseVector3f(j["p3"]);
	p4 = ParseVector3f(j["p4"]);
}

Vector3f Rectangle::GetIntersectionPoint(Ray ray, float t)
{
	return ray.origin + t * ray.direction;
}

HitRecord Rectangle::hit(Ray ray)
{
	vector<Triangle> triangles = vector<Triangle>(0);
	triangles.push_back(Triangle(p1, p2, p3));
	triangles.push_back(Triangle(p3, p4, p1));

	for each (Triangle tri in triangles)
	{
		Vector3f normal = (tri.C - tri.B).cross(tri.A - tri.B).normalized();
		float t = ((tri.A - ray.origin).dot(normal)) / (ray.direction.dot(normal));
		Vector3f P = (ray.origin + t * ray.direction);

		//check if in triangle
		float Sbc = (((tri.C - tri.B).cross(P - tri.B)).dot(normal));
		float Sca = (((tri.A - tri.C).cross(P - tri.C)).dot(normal));
		float Sab = (((tri.B - tri.A).cross(P - tri.A)).dot(normal));

		if (Sbc >= 0 && Sca >= 0 && Sab >= 0)
		{
			return HitRecord(this, t, normal);
		}
	}

	return HitRecord(this,Infinity,Vector3f());
}
