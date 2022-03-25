#include "Scene.h"
#include <Eigen/Core>
#include <Eigen/Dense>
#include <string>
#include "../external/json.hpp"
#include <iostream>
#include "HitRecord.h"
#include "Geometry.h"
#include "Material.h"


using std::string;
using std::cout;
using nlohmann::json;
using namespace Eigen;

Vector3f ParseVector3f(json j);

Scene::Scene()
{
}

Scene::Scene(json j)
{
	if (ValidateFile(j) == "") {

		filename = j["filename"].get<string>();

		for (int i = 0; i < size.size(); i ++)
		{
			size[i] = j["size"][i].get<unsigned int>();
		}

		camera = Camera(j);

		ai = ParseVector3f(j["ai"]);
		bkc = ParseVector3f(j["bkc"]);

		//optional params
		if (!j["speedup"].is_null())
		{
			speedup = j["speedup"].get<bool>();
		}

		if (!j["raysperpixel"].is_null())
		{
			int size = j["raysperpixel"].size();
			raysperpixel = new unsigned int[size];
			for (int i = 0; i < size; i++)
			{
				raysperpixel[i] = j["raysperpixel"][i].get<unsigned int>();
			}
		}

		if (!j["antialiasing"].is_null())
		{
			antialiasing = j["antialiasing"].get<bool>();
		}

		if (!j["twosiderender"].is_null())
		{
			twosiderender = j["twosiderender"].get<bool>();
		}

		if (!j["globalillum"].is_null())
		{
			globalillum = j["globalillum"].get<bool>();
		}

		if (!j["maxbounces"].is_null())
		{
			maxbounces = j["maxbounces"].get<int>();
		}

		if (!j["probterminate"].is_null())
		{
			probterminate = j["probterminate"].get<float>();
		}
	}
	else
	{
		cout << "invalid input file";
	}

}

string Scene::ValidateFile(json j)
{
	return "";
}

Vector3f Scene::illuminate(HitRecord hrec)
{
	return ai.cwiseProduct(hrec.hit->material.ka * hrec.hit->material.ac);
}
