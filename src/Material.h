#pragma once
#include <Eigen/Core>
#include "../external/json.hpp"
using namespace Eigen;

using namespace nlohmann;

class Material
{
public:
	Material();
	Material(json j);

	/// <summary>
	/// Evaluate the color of the geometry
	/// </summary>
	/// <param name="l"> light direction </param>
	/// <param name="v"> viewing ray </param>
	/// <param name="n"> surface normal </param>
	/// <returns></returns>
	Vector3f evaluate(Vector3f l, Vector3f v, Vector3f n, Vector3f id, Vector3f is);
 
	Matrix<float, 3, 1> ac;
	Matrix<float, 3, 1> dc;
	Matrix<float, 3, 1> sc;

	float ka;
	float kd;
	float ks;

	float pc;
};
