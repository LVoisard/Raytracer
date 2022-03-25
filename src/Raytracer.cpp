#define _USE_MATH_DEFINES



#include "RayTracer.h"
#include <Eigen/Core>
#include <Eigen/Dense>
#include "../external/json.hpp"
#include "../external/simpleppm.h"
#include <iostream>
#include <vector>
#include "Sphere.h"
#include "Rectangle.h"
#include "AreaLight.h"
#include "PointLight.h"
#include "Ray.h"
#include "HitRecord.h"
#include <algorithm>
#include <chrono>

using namespace std;
using nlohmann::json;

Vector3f CalculateRayDirection(Matrix<int, 2, 1>, Vector3f, Vector3f, Vector3f, float, float, float);
float Distance(Vector3f, Vector3f);

RayTracer::RayTracer()
{
}

RayTracer::RayTracer(json j)
{
	srand(time(0));
	for (auto scene : j["output"])
	{
		this->scenes.push_back(Scene(scene));
	}
	for (auto object : j["geometry"])
	{
		if( object["visible"].is_null() || (!object["visible"].is_null() && object["visible"]))
		if (!object["type"].is_null())
		{
			if (object["type"] == "sphere")
			{
				this->objects.push_back(new Sphere(object));
			}
			else if (object["type"] == "rectangle")
			{
				this->objects.push_back(new Rectangle(object));
			}
		}
	}
	for (auto light : j["light"])
	{
		if (!light["type"].is_null())
		{
			if (light["type"] == "point")
			{
				this->lights.push_back(new PointLight(light));
			}
			else if (light["type"] == "area")
			{
				this->lights.push_back(new AreaLight(light));
			}
		}
	}
}

void RayTracer::run()
{
	for (Scene scene : this->scenes)
	{
		save_scene(scene);
	}
}

void RayTracer::load_objects()
{
}

void RayTracer::load_light()
{
}

void RayTracer::save_scene(Scene scene)
{
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();

	int dimx = scene.size.x();
	int dimy = scene.size.y();

	vector<double> buffer(3 * dimx * dimy);


	// backface culling
	if (true)
	{
		vector<Geometry*> renderList;
		//std::remove(objects.begin(), objects.end(), value)
		//objects.erase(remove_if(begin(objects), end(objects), std::mem_fn(&Foo::flag)), end(v));
		//int size = objects.size();
		for (int i = 0; i < objects.size(); i++)
		{
			Rectangle* rec = dynamic_cast<Rectangle*>(objects.at(i));
			Sphere* s = dynamic_cast<Sphere*>(objects.at(i));

			if (rec != nullptr)
			{
				Vector3f direction = (scene.camera.centre - rec->p1).normalized();
				float angle = direction.dot(rec->normal);
				if (angle >= 0)
				{
					renderList.push_back(rec);
				}
			}
			else if (s != nullptr)
			{
				renderList.push_back(s);
			}
		}

		objects = renderList;
	}


	if (!scene.globalillum)
	{
		int antialiasingSamples = 1;
		if (scene.antialiasing)
		{
			antialiasingSamples = scene.raysperpixel[0];
		}

		for (int y = 0; y < dimy; y++)
		{
			for (int x = 0; x < dimx; x++)
			{
				Vector3f color = scene.bkc;
				Vector3f ambientColor = Vector3f(0, 0, 0);
				Vector3f lightingColor = Vector3f(0, 0, 0);


				for (int i = 0; i < antialiasingSamples; i++)
				{
					float randOffsetX = scene.antialiasing ? (float)rand() / RAND_MAX : 0;
					float randOffsetY = scene.antialiasing ? (float)rand() / RAND_MAX : 0;

					Ray ray = scene.camera.CalculateViewingRay(x + randOffsetX, y + randOffsetY, scene.antialiasing);

					// initialize hit record with t = Infinity (no intersection)
					// Get the intersection point for every geometry on the pixel
#pragma region Intersecting Geometries
					HitRecord closestHit = HitRecord();
					for each (Geometry * g in objects)
					{
						HitRecord rec = g->hit(ray);

						if (rec.t != Infinity)
						{
							if (closestHit.t == Infinity || (rec.t < closestHit.t && rec.t > 0))
							{
								closestHit = rec;
							}
						}
					}
#pragma endregion

					if (closestHit.t != Infinity)
					{
						ambientColor = scene.illuminate(closestHit);
						for each (Light * light in lights)
						{
							if (!light->IsShaded(ray, objects, closestHit))
								lightingColor += light->illuminate(ray, closestHit);
						}
						color = ambientColor + lightingColor / antialiasingSamples;
					}

					color[0] = std::max(color[0], 0.0f);
					color[1] = std::max(color[1], 0.0f);
					color[2] = std::max(color[2], 0.0f);

					color[0] = std::min(color[0], 1.0f);
					color[1] = std::min(color[1], 1.0f);
					color[2] = std::min(color[2], 1.0f);

					buffer[3 * y * dimx + 3 * x + 0] = color[0];
					buffer[3 * y * dimx + 3 * x + 1] = color[1];
					buffer[3 * y * dimx + 3 * x + 2] = color[2];
				}
			}
		}
	}
	else
	{
		for (int y = 0; y < dimy; y++)
		{
			for (int x = 0; x < dimx; x++)
			{
				Vector3f color = scene.bkc;
				Vector3f ambientColor = Vector3f(0, 0, 0);
				Vector3f lightingColor = Vector3f(0, 0, 0);

				bool isObjectHit = false;

				for each (Light * light in lights)
				{
					for (int i = 0; i < scene.raysperpixel[1]; i ++)
					{
						for (int j = 0; j < scene.raysperpixel[1]; j ++)
						{
							std::chrono::time_point<std::chrono::system_clock> pixelStart, pixelEnd;
							pixelStart = std::chrono::system_clock::now();

							Ray ray = scene.camera.CalculateViewingRay(x + (float) (j / scene.raysperpixel[1]) / 2, y + (float)(i / scene.raysperpixel[1]) / 2, scene.antialiasing);

							//cout << "Created Ray: " << (std::chrono::system_clock::now() - pixelStart).count() / 1000 << endl;

							if (!light->use)
								continue;
							//cout << "Checked Light: " << (std::chrono::system_clock::now() - pixelStart).count() / 1000 << endl;
							HitRecord closestHit = HitRecord();
							for each (Geometry * g in objects)
							{
								HitRecord rec = g->hit(ray);
								//cout << "Intersected: " << (std::chrono::system_clock::now() - pixelStart).count() / 1000 << endl;

								if (rec.t != Infinity)
								{
									if (closestHit.t == Infinity || (rec.t < closestHit.t && rec.t > 0))
									{
										closestHit = rec;
									}
								}
								//cout << "Looking in objects: " << (std::chrono::system_clock::now() - pixelStart).count() / 1000 << endl;

							}

							//cout << "Found Closest Object: " << (std::chrono::system_clock::now() - pixelStart).count() / 1000 << endl;

							if (closestHit.t != Infinity)
							{
								lightingColor += CalculatePath(ray, ray, closestHit, objects, light, scene.maxbounces, scene.probterminate, scene.raysperpixel[1]);
								isObjectHit = true;
							}
							//cout << "Overall time for pixel sample " << (std::chrono::system_clock::now() - pixelStart).count() / 1000 << endl;
						}
					}
				}

				if(isObjectHit)
					color = lightingColor / (scene.raysperpixel[1] * scene.raysperpixel[1]);

				color[0] = std::max(color[0], 0.0f);
				color[1] = std::max(color[1], 0.0f);
				color[2] = std::max(color[2], 0.0f);

				color[0] = std::min(color[0], 1.0f);
				color[1] = std::min(color[1], 1.0f);
				color[2] = std::min(color[2], 1.0f);

				buffer[3 * y * dimx + 3 * x + 0] = color[0];
				buffer[3 * y * dimx + 3 * x + 1] = color[1];
				buffer[3 * y * dimx + 3 * x + 2] = color[2];

			}
			cout << (float)(y) / (float)(dimy) * 100.0f << "%" << endl;
		}
	}

	end = std::chrono::system_clock::now();
	std::chrono::duration<double> time = end - start;
	cout << "Total Time: " << time.count() / 60 << " Minutes";

	save_ppm(scene.filename, buffer, dimx, dimy);
}

void GetLocalCoordinatesInHemisphere(HitRecord hrec, Vector3f& local2d, Vector3f& right)
{	
	if (std::fabs(hrec.n.x()) > std::fabs(hrec.n.y()))
	{
		local2d = Vector3f(hrec.n.z(), 0, -hrec.n.x()) / sqrtf(hrec.n.x() * hrec.n.x() + hrec.n.z() * hrec.n.z());
	}
	else
	{
		local2d = Vector3f(0, -hrec.n.z(), hrec.n.y()) / sqrtf(hrec.n.y() * hrec.n.y() + hrec.n.z() * hrec.n.z());
	}

	right = local2d.cross(hrec.n);
}

Vector3f GetUniformSampleInHemisphere(float val1, float val2)
{
	float sinTheta = sqrtf(1 - val1 * val1);
	float cosTheta = sqrtf(1 - val2 * val2);
	float mag = 2 * M_PI * val2;
	float x = sinTheta * cosf(mag);
	float z = sinTheta * sinf(mag);
	return Vector3f(x, val1, z);
}

Vector3f RayTracer::CalculatePath(Ray viewingRay, Ray ray, HitRecord lastHit, vector<Geometry*> objects, Light* light, int bouncesLeft, float probTerminate, int samples)
{
	float prob = (float)rand() / RAND_MAX;
	// IF the path terminates, send a ray to the light or shade and then traceback
	if (probTerminate > prob || bouncesLeft <= 0)
	{
		return !light->IsShaded(ray, objects, lastHit)* light->illuminate(viewingRay, lastHit);
	}

	HitRecord closestHit = HitRecord();
	for each (Geometry * g in objects)
	{
		HitRecord rec = g->hit(ray);

		if (rec.t != Infinity)
		{
			if (closestHit.t == Infinity || (rec.t < closestHit.t && rec.t > 0))
			{
				closestHit = rec;
			}
		}
	}

	if (closestHit.t > 0)
	{
		Vector3f local2d, right;
		GetLocalCoordinatesInHemisphere(closestHit, local2d, right);

		float val1 = (float)rand() / RAND_MAX;
		float val2 = (float)rand() / RAND_MAX;

		Vector3f sample = GetUniformSampleInHemisphere(val1, val2);
		Vector3f sampleInHemisphere = Vector3f(
			sample.x() * right.x() + sample.y() * closestHit.n.x() + sample.z() * local2d.x(),
			sample.x() * right.y() + sample.y() * closestHit.n.y() + sample.z() * local2d.y(),
			sample.x() * right.z() + sample.y() * closestHit.n.z() + sample.z() * local2d.z());

		Ray sampleRay = Ray(ray.evaluate(closestHit.t) + sampleInHemisphere, sampleInHemisphere);

		Vector3f indirectLighting = CalculatePath(viewingRay, sampleRay, closestHit, objects, light, bouncesLeft - 1, probTerminate, samples) / ( 1 / (2 * M_PI));

		Vector3f directLight = !light->IsShaded(ray, objects, closestHit) * light->illuminate(viewingRay, closestHit);

		Vector3f col = (directLight / M_PI + 2 * indirectLighting);

		col.x() = min(1.0f, col.x());
		col.y() = min(1.0f, col.y());
		col.z() = min(1.0f, col.z());

		col.x() = max(0.0f, col.x());
		col.y() = max(0.0f, col.y());
		col.z() = max(0.0f, col.z());

		return col;
	}
	else
	{
		// return the light
		return CalculatePath(viewingRay, ray, lastHit, objects, light, 0, probTerminate, samples);
	}
}