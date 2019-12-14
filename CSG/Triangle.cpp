#include <iostream>
#include <cmath>
#include "Triangle.h"

using namespace std;

//DEFAULT CONSTRUCTOR
Triangle::Triangle()
{
	p1 = { 0.0f, 0.0f, 0.0f, 1.0f };
	p2 = { 0.0f, 0.0f, 0.0f, 1.0f };
	p3 = { 0.0f, 0.0f, 0.0f, 1.0f };
	normal = { 0.0f, 0.0f, 0.0f, 1.0f };
}

//OVERLOAD CONSTRUCTOR (from three Points)
Triangle::Triangle(const Vector4Df &nPt1, const Vector4Df &nPt2, const Vector4Df &nPt3)
{
	p1 = nPt1;
	p2 = nPt2;
	p3 = nPt3;
	normal = Normal().to_unitary();
}

//OVERLOAD CONSTRUCTOR (from another Triangle)
Triangle::Triangle(const Triangle& other)
{
	p1 = other.p1;
	p2 = other.p2;
	p3 = other.p3;
	normal = Normal().to_unitary();
}

//DESTRUCTOR
Triangle::~Triangle()
{

}

//ALLOCATION (from three Points)
void Triangle::Set(const Vector4Df &nPt1, const Vector4Df &nPt2, const Vector4Df &nPt3)
{
	p1 = nPt1;
	p2 = nPt2;
	p3 = nPt3;
	normal = Normal().to_unitary();
}

//ALLOCATION (from another Triangle)
void Triangle::Set(const Triangle& other)
{
	p1 = other.p1;
	p2 = other.p2;
	p3 = other.p3;
	normal = Normal().to_unitary();
}

//AREA
float Triangle::Area() const
{
	return Normal(p1, p2, p3).length() / 2.0f;
}

//NORMAL
Vector4Df Triangle::Normal() const
{
	return Normal(p1, p2, p3);
}
Vector4Df Triangle::Normal(const Vector4Df &nPt1, const Vector4Df &nPt2, const Vector4Df &nPt3)
{
	return cross(nPt2 - nPt1, nPt3 - nPt1);
}

//CONTAINS (by Barycentric coordinate system)
bool Triangle::Contains(const Vector4Df &pTest) const
{
	float s1 = dot(Normal(p1, p2, pTest), Normal());
	float s2 = dot(Normal(p2, p3, pTest), Normal());
	float s3 = dot(Normal(p3, p1, pTest), Normal());

	if (s1 < SMALL_NUMBER) return false;
	else if (s2 < SMALL_NUMBER) return false;
	else if (s3 < SMALL_NUMBER) return false;

	return true;
}
