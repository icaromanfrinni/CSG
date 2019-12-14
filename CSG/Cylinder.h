#pragma once

//
// Cylinder Class
//

//System Include Files
#include <iostream>

//Project Include Files
#include "Geometry.h"
#include "Linear_Algebra.h"
#include "Ray.h"

class Cylinder : public Geometry
{
public:
	float height, radius;
	Vector4Df base_center; // Cylinder base center
	Vector4Df direction;  // Cylinder direction vector
	int id;

//public:
	Cylinder();
	Cylinder(float height, float radius, Vector4Df base_center, Vector4Df direction);
	~Cylinder();

	RayCollisionList CollideAll(const std::vector<Ray> &ray);
	float CollideClosest(const Ray &ray) const;
	RayCollisionList Collide(const Ray &ray); 

	Vector4Df getNormal(const Vector4Df &point);
	box getBB();
};

