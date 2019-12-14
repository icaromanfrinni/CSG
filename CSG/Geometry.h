#pragma once

//
// Base class for geometry
//

#include <vector>

#include "Ray.h"
#include "Box.h"

class Geometry
{
public:
	Geometry();
	virtual ~Geometry() = 0;

	virtual float CollideClosest(const Ray &ray) const = 0;//return the closest collision distance of a ray and the geometry
	virtual RayCollisionList CollideAll(const std::vector<Ray> &ray) = 0;//return all the colisions of a set of rays
	virtual RayCollisionList Collide(const Ray &ray) = 0;//return all collisions of a ray

	virtual Vector4Df getNormal(const Vector4Df &point) = 0; // returns the normal vector at point on a surface
	virtual box getBB() = 0;
};

