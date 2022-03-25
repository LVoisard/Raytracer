#pragma once
#include "../external/json.hpp"
#include "Scene.h"
#include "Geometry.h"
#include "Light.h"
#include <vector>
#include <Eigen/Core>
#include <Eigen/Dense>



using nlohmann::json;
using std::vector;

class RayTracer
{
public:
	RayTracer();
	RayTracer(json);
	void run();
private:

	vector<Scene> scenes;
	vector<Geometry*> objects;
	vector<Light*> lights;
	void load_objects();
	void load_light();
	void save_scene(Scene scene);
	Vector3f CalculatePath(Ray viewingRay, Ray ray, HitRecord lastHit, vector<Geometry*> objects, Light* light, int bounceLeft, float probTerminate, int samples);
};