#pragma once

#include <string>
#include <Eigen/Core>
#include "../external/json.hpp"
#include <Camera.h>
#include "HitRecord.h"
#include "Geometry.h"
#include "Material.h"


using std::string;
using nlohmann::json;
using namespace Eigen;

class Scene 
{
public:
	Scene();
	Scene(json);
	string ValidateFile(json);

	Vector3f illuminate(HitRecord hrec);

	string filename;
	unsigned int size[2];
	Camera camera;	
	Vector3f ai;
	Vector3f bkc;

	//optional params

	unsigned int speedup;
	unsigned int* raysperpixel;
	bool antialiasing;
	bool twosiderender;

	bool globalillum;
	// max light bounces
	int maxbounces;
	// Odds of the path to end
	float probterminate;
};
