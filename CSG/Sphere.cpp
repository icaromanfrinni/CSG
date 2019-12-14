#include "Sphere.h"



Sphere::Sphere()
{
	center = { 0.0f, 0.0f, 0.0f, 1.0f };
	radius = 1.0f;
	id = 0;
}

Sphere::Sphere(Vector4Df C, float R)
{
	center = C;
	radius = R;
	id = 0;
}

Sphere::~Sphere()
{
}

RayCollisionList Sphere::CollideAll(const std::vector<Ray> &ray)
{
	//Colisions
	RayCollisionList col, col_aux;

	for (unsigned i = 0; i < ray.size(); i++)
	{
		col_aux = this->Collide(ray.at(i));
		col.collisions.insert(col.collisions.end(), col_aux.collisions.begin(), col_aux.collisions.end());
	}

	return col;
}

float Sphere::CollideClosest(const Ray &ray) const
{
	Vector4Df d = ray.direction.to_unitary();	// Vector that defines the Ray direction
	Vector4Df W = ray.origin - center;			// The difference of P0 - C

	// Coefficients of the equation
	//const float A = 1.0f;//considering that the direction is already normalized
	float B = 2.0f * dot(W, d);
	float C = dot(W, W) - (radius * radius);
	// Discriminant
	float Delta = (B*B) - (4.0f * C);

	if (Delta < 0.0f) return INFINITY;

	//First point
	return (B + sqrtf(Delta)) * (-0.5f);
}

RayCollisionList Sphere::Collide(const Ray &ray)
{
	//Colisions
	RayCollisionList col;
	Collision P;

	Vector4Df d = ray.direction.to_unitary();	// Vector that defines the Ray direction
	Vector4Df W = ray.origin - center;			// The difference of P0 - C

	// Coefficients of the equation
	float A = dot(d, d);
	float B = 2 * dot(W, d);
	float C = dot(W, W) - (radius * radius);
	// Discriminant
	float Delta = (B*B) - (4 * A*C);

	if (Delta == 0.0) // One intersection
	{
		P.t = (-B) / (2 * A);
		P.point = ray.origin + (d * P.t);
		col.collisions.push_back(P);
	}
	else if (Delta > 0.0) // Two intersections
	{
		// First point
		P.t = (-B - sqrtf(Delta)) / (2 * A);
		P.point = ray.origin + (d * P.t);
		if (P.t >= 0) col.collisions.push_back(P); // is in front
		// Second point
		P.t = (-B + sqrtf(Delta)) / (2 * A);
		P.point = ray.origin + (d * P.t);
		if (P.t >= 0) col.collisions.push_back(P); // is in front
	}

	return col;
}

Vector4Df Sphere::getNormal(const Vector4Df &point)
{
	Vector4Df n = point - center;
	n.normalize();
	return n;
}

box Sphere::getBB()
{
	box BB;
	BB.point = center - Vector4Df{ radius, radius, radius, 0.0f };
	BB.size = 2.0f * radius;
	return BB;
}