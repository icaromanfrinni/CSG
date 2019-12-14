//TODO: - Collision:
//			- Handle a == 0;
//			- Handle Base Intersection

#include "Cone.h"

Cone::Cone()
{
	height = 1.0f;
	radius = 1.0f;
	base_center = { 0.0f, 0.0f, 0.0f, 1.0f };
	direction = { 0.0f, 1.0f, 0.0f, 0.0f }; //unitary
	top_vertex = base_center + (direction * height);
	id = 0;
}

Cone::Cone(float height, float radius, Vector4Df base_center, Vector4Df direction)
{
	this->height = height;
	this->radius = radius;
	this->base_center = base_center;
	this->direction = direction.to_unitary();
	this->top_vertex = this->base_center + (this->direction * this->height);
	id = 0;
}

Cone::~Cone()
{

}

void Cone::SetTopVertex()
{
	this->top_vertex = this->base_center + (this->direction * this->height);
}

RayCollisionList Cone::CollideAll(const std::vector<Ray> &ray)
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

//TODO: Implement it
float Cone::CollideClosest(const Ray &ray) const {

	float col;

	const float d_dot_n = dot_simd(ray.direction, direction);
	//const float d_dot_d = dot_simd(ray.direction, ray.direction);
	//const float d_dot_d = 1.0f; //changed to recieve 1.0f considering that ray.direction has lenght one

	const Vector4Df &v = top_vertex - ray.origin; // Vector between the Cone top vertex and the ray origin.

	const float v_dot_n = dot_simd(v, direction);

	//Aux Variables
	const float r_2 = this->radius * this->radius;
	const float h_2 = this->height * this->height;
	const float cos_alfa_2 = (h_2 / (h_2 + r_2));

	//Equantion Constants
	const float a = (d_dot_n*d_dot_n) - (cos_alfa_2);
	const float b = ((dot_simd(v, ray.direction)*cos_alfa_2) - (v_dot_n * d_dot_n));
	const float c = (v_dot_n*v_dot_n) - (dot_simd(v, v)*cos_alfa_2);
	//Delta
	const float delta = (b*b) - (a*c);

	if (a != 0.0f && delta >= 0.0f) {
		const float distance = (sqrtf(delta) - b) / a;
		const Vector4Df &p = ray.origin + (ray.direction * distance); // Intersection Point
		const float p_projection = dot_simd((top_vertex - p), direction); //Projection of the point P on the cone axis
		if (p_projection >= 0.0f && p_projection <= this->height) { // Does the ray hit the cone?
			col = distance;
			return col;
		}
	}

	//TODO: It must be implmented only if one of the cone interception is not valid or 'a' is less than 0.
	//Base
	const Vector4Df &base_dir = this->direction * (-1.0f);
	//OBS.: Base Center is a point on the base plan.
	const float distance = dot_simd((this->base_center - ray.origin), base_dir) / (dot_simd(ray.direction, base_dir));
	const Vector4Df &p = ray.origin + (ray.direction * distance); // Intersection Point
	const float int_to_center = (p - this->base_center).lengthsq(); // Distance of the intersection point from the base center.

	if (int_to_center <= r_2) {//The point intercept tha base iff its distance from the center is less than the radius.
		col = distance;
		return col;
	}

	col = INFINITY;

	return col;
}

RayCollisionList Cone::Collide(const Ray &ray)
{
	//Colisions
	RayCollisionList col;
	Collision t;

	t.geometry = this;

	//Aux Variables
	const float r_2 = this->radius * this->radius;
	const float h_2 = this->height * this->height;
	const float cos_alfa_2 = (h_2 / (h_2 + r_2));

	float d_dot_n = dot(ray.direction, this->direction);
	float d_dot_d = dot(ray.direction, ray.direction);

	Vector4Df v = this->top_vertex - ray.origin; // Vector between the Cone top vertex and the ray origin.

	float v_dot_n = dot(v, this->direction);

	//Equantion Constants
	float a = (d_dot_n*d_dot_n) - (d_dot_d * cos_alfa_2);
	float b = 2 * ((dot(v, ray.direction)*cos_alfa_2) - (v_dot_n * d_dot_n));
	float c = (v_dot_n*v_dot_n) - (dot(v, v)*cos_alfa_2);
	//Delta
	float delta = (b*b) - (4 * a*c);

	if (a != 0)
	{

		if (delta == 0)
		{ // One intersection
			t.t = (-b / (2 * a));
			Vector4Df p = ray.origin + (ray.direction * t.t); // Intersection Point
			float p_projection = dot((this->top_vertex - p), this->direction); //Projection of the point P on the cone axis
			if (p_projection >= 0 && p_projection <= this->height) { // Does the ray hit the cone?
				t.point = p;
				col.collisions.push_back(t);
			}
		}
		else if (delta > 0)
		{
			delta = sqrtf(delta);
			//First Point
			t.t = ((-1)*(delta + b)) / (2 * a);
			Vector4Df p = ray.origin + (ray.direction * t.t); // Intersection Point
			float p_projection = dot((this->top_vertex - p), this->direction); //Projection of the point P on the cone axis
			if (p_projection >= 0 && p_projection <= this->height)
			{ // Does the ray hit the cone?
				t.point = p;
				col.collisions.push_back(t);
			}

			//Second Point
			t.t = (delta - b) / (2 * a);
			p = ray.origin + (ray.direction * t.t); // Intersection Point
			p_projection = dot((this->top_vertex - p), this->direction); //Projection of the point P on the cone axis
			if (p_projection >= 0 && p_projection <= this->height)
			{ // Does the ray hit the cone?
				t.point = p;
				col.collisions.push_back(t);
			}
		}
	}

	//TODO: It must be implmented only if one of the cone interception is not valid or 'a' is less than 0.
	//Base
	Vector4Df base_dir = this->direction * (-1);
	//OBS.: Base Center is a point on the base plan.
	t.t = dot((this->base_center - ray.origin), base_dir) / (dot(ray.direction, base_dir));
	Vector4Df p = ray.origin + (ray.direction * t.t); // Intersection Point
	float int_to_center = (p - this->base_center).length(); // Distance of the intersection point from the base center.

	if (int_to_center <= this->radius)
	{//The point intercept tha base iff its distance from the center is less than the radius.
		t.point = p;
		col.collisions.push_back(t);
	}

	//reorder points from near to far
	if (col.collisions.size() > 1)
	{
		if (col.collisions[0].t > col.collisions[1].t)
		{
			Collision nearP = col.collisions[1];
			col.collisions[1] = col.collisions[0];
			col.collisions[0] = nearP;
		}
	}

	return col;
}

/*
Vector4Df Cone::getNormal(const Vector4Df &point)
{
	Vector4Df n, vg, vt;
	if (point.w < 0.0f) {
		//Vector4Df p = point;
		//p.w = 1.0f;
		//Vector4Df a = (p - top_vertex);
		//Vector4Df b = (direction * point.w);
		//n = (a - b).to_unitary();
		vg = top_vertex - point;
		vt = cross(vg, direction);

		n = cross(vt, vg).to_unitary();
	}
	else {
		n = direction * (-1.0f);
	}

	return n;
}
*/

Vector4Df Cone::getNormal(const Vector4Df &point)
{
	Vector4Df CP = point - base_center;

	if (CP.length() <= radius && fabs(dot(direction, CP)) < SMALL_NUMBER )
	{
		return direction * (-1.0f);
	}

	float alfa = atanf(radius / height);
	Vector4Df n = CP - (direction * dot(CP, direction));
	n = n / cosf(alfa);

	n.normalize();
	return n;
}

box Cone::getBB()
{
	box BB;
	
	BB.point = base_center - Vector4Df{ radius, 0.0f, radius, 0.0f };
	BB.size = height;
	if (2.0f * radius > BB.size)
		BB.size = 2.0f * radius;

	return BB;
}

void Cone::transform(Matrix4 m) {
	base_center = m * base_center;
	direction = m * direction;
	direction.to_unitary();
	top_vertex = base_center + (direction * height);
}