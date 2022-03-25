#include "Material.h"
#include "HitRecord.h"
#include "../external/json.hpp"
#include <Eigen/Core>

Vector3f ParseVector3f(json j);

Material::Material()
{};

Material::Material(json j)
{
	// ambient color
	ac = ParseVector3f(j["ac"]);
	// diffuse color
	dc = ParseVector3f(j["dc"]);
	// specular color
	sc = ParseVector3f(j["sc"]);

	// ambient coefficient
	ka = j["ka"].get<float>();
	// diffuse coefficient
	kd = j["kd"].get<float>();
	// specular coefficient
	ks = j["ks"].get<float>();

	// phong coefficient
	pc = j["pc"].get<float>();
}

Vector3f Material::evaluate(Vector3f l, Vector3f v, Vector3f n, Vector3f id, Vector3f is)
{
	Vector3f diffuse = Vector3f(0, 0, 0), specular = Vector3f(0, 0, 0);

	diffuse = id.cwiseProduct(dc) * kd * n.dot(l);

	diffuse[0] = std::max(diffuse[0], 0.0f);
	diffuse[1] = std::max(diffuse[1], 0.0f);
	diffuse[2] = std::max(diffuse[2], 0.0f);

	 Vector3f R = (2 * n * (n.dot(l)) - l);
	 float specAngle = std::max(0.0f, R.dot(v));
	Vector3f H = (l + v) / (l + v).norm();

	specular = is.cwiseProduct(sc * ks * powf(specAngle, pc));
	//specular = is.cwiseProduct(sc * ks * powf(H.dot(n), pc));

	specular[0] = std::max(specular[0], 0.0f);
	specular[1] = std::max(specular[1], 0.0f);
	specular[2] = std::max(specular[2], 0.0f);


	return diffuse + specular;
}
