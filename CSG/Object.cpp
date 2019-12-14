#include "Object.h"


Object::Object()
{
}
Object::Object(Geometry *_geometry) : geometry(_geometry)
{
	name = "untitled"; // default name
	material = new Material(Vector4Df{ 1.0f, 1.0f, 1.0f, 0 }, Vector4Df{ 0.0f, 0.0f, 0.0f, 0 }, Vector4Df{ 0.0f, 0.0f, 0.0f, 0 }, 100.0f); // default color
}

Object::Object(const string &_name, Material * _material, Geometry *_geometry) : name(_name), geometry(_geometry), material(_material), visible(true)
{
}

Object::~Object()
{
}

float Object::Collide(register const Ray &ray) const {
	//RayCollisionList Coll = geometry->Collide(ray);
	//if (Coll.collisions.size() == 0)return INFINITY;
	//return Coll.collisions[0].distance;
	return geometry->CollideClosest(ray);
}

RayCollisionList Object::CollideAll(const Ray &ray) const {
	return geometry->Collide(ray);
}

string Object::getName() const {
	return name;
}

Geometry* Object::getGeometry() const {
	return geometry;
}

Material* Object::getMaterial() const {
	return material;
}

Vector4Df Object::getNormalVector(const Vector4Df &point) const {
	return geometry->getNormal(point);
}

box Object::getBoundingBox() const
{
	return geometry->getBB();
}