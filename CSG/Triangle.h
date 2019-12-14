#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Linear_Algebra.h"
#include "Ray.h"

class Triangle
{
public:
	//DEFAULT CONSTRUCTOR
	Triangle();

	//OVERLOAD CONSTRUCTOR (from three Points)
	Triangle(const Vector4Df &nPt1, const Vector4Df &nPt2, const Vector4Df &nPt3);
	//OVERLOAD CONSTRUCTOR (from another Triangle)
	Triangle(const Triangle& other);
	//DESTRUCTOR
	~Triangle();

	//ALLOCATION (from three Points)
	void Set(const Vector4Df &nPt1, const Vector4Df &nPt2, const Vector4Df &nPt3);
	//ALLOCATION (from another Triangle)
	void Set(const Triangle &other);

	//AREA
	float Area() const;
	//NORMAL
	Vector4Df Normal() const;
	static Vector4Df Normal(const Vector4Df &nPt1, const Vector4Df &nPt2, const Vector4Df &nPt3);
	//CONTAINS (by Barycentric coordinate system)
	bool Contains(const Vector4Df &pTest) const;

	//VARIABLES
	Vector4Df p1, p2, p3, normal;
};

#endif // TRIANGLE_H
