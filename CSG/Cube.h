#pragma once
#include "Geometry.h"
#include "Triangle.h"

class Cube : public Geometry
{
public:
	Triangle faces[12];
	Vector4Df center;
	float sizeX, sizeY, sizeZ;
	int id;
//public:
	Cube();
	Cube(Vector4Df C, float sX, float sY, float sZ);
	~Cube();

	void SetVertexFace();

	RayCollisionList CollideAll(const std::vector<Ray> &ray);
	float CollideClosest(const Ray &ray) const;
	RayCollisionList Collide(const Ray &ray);

	Vector4Df getNormal(const Vector4Df &point);
	box getBB();
};