#pragma once
#include "Linear_Algebra.h"
#include "Geometry.h"

class Sphere : public Geometry
{
public:
	Vector4Df center;	// sphere center
	float radius;		// sphere radius
	int id;

//public:
	Sphere();
	Sphere(Vector4Df C, float R);
	~Sphere();

	RayCollisionList CollideAll(const std::vector<Ray> &ray);
	float CollideClosest(const Ray &ray) const;
	RayCollisionList Collide(const Ray &ray); // return the collisions of a ray

	Vector4Df getNormal(const Vector4Df &point);
	box getBB();
};
