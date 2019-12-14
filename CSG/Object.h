#pragma once
#include <string>
#include "Geometry.h"
#include "Material.h"
#include "Linear_Algebra.h"

using namespace std;

class Object
{
	Geometry *geometry;
	Material *material;
public:
	string name;
	bool visible = false;
	int op; //boolean oparation

	Object();
	Object(Geometry *_geometry);
	Object(const string &_name, Material * _material, Geometry *_geometry);

	~Object();

	float Collide(register const Ray &ray) const;
	RayCollisionList CollideAll(const Ray &ray) const;

	string getName() const;
	Geometry* getGeometry() const;
	Material* getMaterial() const;
	Vector4Df getNormalVector(const Vector4Df &point) const;
	box getBoundingBox() const;
};

