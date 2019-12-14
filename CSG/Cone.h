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

class Cone : public Geometry
{

public:
	Vector4Df base_center; // Cone base center
	Vector4Df direction;   // Cone direction vector
	Vector4Df top_vertex;  // Cone top vertex
	float height, radius, cos_alfa_2;
	int id;

	Cone();
	Cone(float height, float radius, Vector4Df base_center, Vector4Df direction);
	~Cone();

	void SetTopVertex();

	RayCollisionList CollideAll(const std::vector<Ray> &ray);//return all the colisions
	float CollideClosest(const Ray &ray) const;
	RayCollisionList Collide(const Ray &ray);//return the closest collision 

	Vector4Df getNormal(const Vector4Df &point); // returns the normal vector at point on a surface
	box getBB();

	void transform(Matrix4 m);
};