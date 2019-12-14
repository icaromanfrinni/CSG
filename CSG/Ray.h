#pragma once
#include "Linear_Algebra.h"
#include <vector>
using namespace std;

class Geometry;//foward declaration for geometry

struct Ray
{
	Vector4Df origin, direction;
};

struct Collision
{
	float t;			// distance
	Vector4Df point;	// collision point
	Geometry *geometry;
};

struct RayCollisionList
{
	vector<Collision> collisions;	// collision list of a ray
};