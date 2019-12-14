#include "Cube.h"
#include <iostream>

Cube::Cube()
{
	center = { 0.0f, 0.0f, 0.0f, 1.0f };
	sizeX = 1.0f;
	sizeY = 1.0f;
	sizeZ = 1.0f;
	id = 0;

	// VERTICES
	Vector4Df p1 = center + (Vector4Df{ sizeX,  sizeY,  sizeZ, 0.0f } / 2.0f); //top, front, right
	Vector4Df p2 = center + (Vector4Df{ -sizeX,  sizeY,  sizeZ, 0.0f } / 2.0f); //top, front, left
	Vector4Df p3 = center + (Vector4Df{ -sizeX, -sizeY,  sizeZ, 0.0f } / 2.0f); //bottom, front, left
	Vector4Df p4 = center + (Vector4Df{ sizeX, -sizeY,  sizeZ, 0.0f } / 2.0f); //bottom, front, right
	Vector4Df p5 = center + (Vector4Df{ sizeX,  sizeY, -sizeZ, 0.0f } / 2.0f); //top, back, right
	Vector4Df p6 = center + (Vector4Df{ -sizeX,  sizeY, -sizeZ, 0.0f } / 2.0f); //top, back, left
	Vector4Df p7 = center + (Vector4Df{ -sizeX, -sizeY, -sizeZ, 0.0f } / 2.0f); //bottom, back, left
	Vector4Df p8 = center + (Vector4Df{ sizeX, -sizeY, -sizeZ, 0.0f } / 2.0f); //bottom, back, right

	// FACES
	//Top (ZX plane, +Y)
	faces[0] = Triangle{ p5, p6, p2 };
	faces[1] = Triangle{ p2, p1, p5 };
	//Bottom (ZX plane, -Y)
	faces[2] = Triangle{ p4, p3, p7 };
	faces[3] = Triangle{ p7, p8, p4 };
	//Front (XY plane, +Z)
	faces[4] = Triangle{ p1, p2, p3 };
	faces[5] = Triangle{ p3, p4, p1 };
	//Back (XY plane, -Z)
	faces[6] = Triangle{ p6, p5, p8 };
	faces[7] = Triangle{ p8, p7, p6 };
	//Right (YZ plane, +X)
	faces[8] = Triangle{ p5, p1, p4 };
	faces[9] = Triangle{ p4, p8, p5 };
	//Left (YZ plane, -X)
	faces[10] = Triangle{ p2, p6, p7 };
	faces[11] = Triangle{ p7, p3, p2 };
}

Cube::Cube(Vector4Df C, float sX, float sY, float sZ)
{
	center = C;
	sizeX = sX;
	sizeY = sY;
	sizeZ = sZ;
	id = 0;

	// VERTICES
	Vector4Df p1 = center + (Vector4Df{  sizeX,  sizeY,  sizeZ, 0.0f } / 2.0f); //top, front, right
	Vector4Df p2 = center + (Vector4Df{ -sizeX,  sizeY,  sizeZ, 0.0f } / 2.0f); //top, front, left
	Vector4Df p3 = center + (Vector4Df{ -sizeX, -sizeY,  sizeZ, 0.0f } / 2.0f); //bottom, front, left
	Vector4Df p4 = center + (Vector4Df{  sizeX, -sizeY,  sizeZ, 0.0f } / 2.0f); //bottom, front, right
	Vector4Df p5 = center + (Vector4Df{  sizeX,  sizeY, -sizeZ, 0.0f } / 2.0f); //top, back, right
	Vector4Df p6 = center + (Vector4Df{ -sizeX,  sizeY, -sizeZ, 0.0f } / 2.0f); //top, back, left
	Vector4Df p7 = center + (Vector4Df{ -sizeX, -sizeY, -sizeZ, 0.0f } / 2.0f); //bottom, back, left
	Vector4Df p8 = center + (Vector4Df{  sizeX, -sizeY, -sizeZ, 0.0f } / 2.0f); //bottom, back, right

	// FACES
	//Top (ZX plane, +Y)
	faces[0] = Triangle{ p5, p6, p2 };
	faces[1] = Triangle{ p2, p1, p5 };
	//Bottom (ZX plane, -Y)
	faces[2] = Triangle{ p4, p3, p7 };
	faces[3] = Triangle{ p7, p8, p4 };
	//Front (XY plane, +Z)
	faces[4] = Triangle{ p1, p2, p3 };
	faces[5] = Triangle{ p3, p4, p1 };
	//Back (XY plane, -Z)
	faces[6] = Triangle{ p6, p5, p8 };
	faces[7] = Triangle{ p8, p7, p6 };
	//Right (YZ plane, +X)
	faces[8] = Triangle{ p5, p1, p4 };
	faces[9] = Triangle{ p4, p8, p5 };
	//Left (YZ plane, -X)
	faces[10] = Triangle{ p2, p6, p7 };
	faces[11] = Triangle{ p7, p3, p2 };
}

Cube::~Cube()
{
}

void Cube::SetVertexFace()
{
	// VERTICES
	Vector4Df p1 = center + (Vector4Df{ sizeX,  sizeY,  sizeZ, 0.0f } / 2.0f); //top, front, right
	Vector4Df p2 = center + (Vector4Df{ -sizeX,  sizeY,  sizeZ, 0.0f } / 2.0f); //top, front, left
	Vector4Df p3 = center + (Vector4Df{ -sizeX, -sizeY,  sizeZ, 0.0f } / 2.0f); //bottom, front, left
	Vector4Df p4 = center + (Vector4Df{ sizeX, -sizeY,  sizeZ, 0.0f } / 2.0f); //bottom, front, right
	Vector4Df p5 = center + (Vector4Df{ sizeX,  sizeY, -sizeZ, 0.0f } / 2.0f); //top, back, right
	Vector4Df p6 = center + (Vector4Df{ -sizeX,  sizeY, -sizeZ, 0.0f } / 2.0f); //top, back, left
	Vector4Df p7 = center + (Vector4Df{ -sizeX, -sizeY, -sizeZ, 0.0f } / 2.0f); //bottom, back, left
	Vector4Df p8 = center + (Vector4Df{ sizeX, -sizeY, -sizeZ, 0.0f } / 2.0f); //bottom, back, right

	// FACES
	//Top (ZX plane, +Y)
	faces[0] = Triangle{ p5, p6, p2 };
	faces[1] = Triangle{ p2, p1, p5 };
	//Bottom (ZX plane, -Y)
	faces[2] = Triangle{ p4, p3, p7 };
	faces[3] = Triangle{ p7, p8, p4 };
	//Front (XY plane, +Z)
	faces[4] = Triangle{ p1, p2, p3 };
	faces[5] = Triangle{ p3, p4, p1 };
	//Back (XY plane, -Z)
	faces[6] = Triangle{ p6, p5, p8 };
	faces[7] = Triangle{ p8, p7, p6 };
	//Right (YZ plane, +X)
	faces[8] = Triangle{ p5, p1, p4 };
	faces[9] = Triangle{ p4, p8, p5 };
	//Left (YZ plane, -X)
	faces[10] = Triangle{ p2, p6, p7 };
	faces[11] = Triangle{ p7, p3, p2 };
}

RayCollisionList Cube::CollideAll(const std::vector<Ray> &ray)
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

float Cube::CollideClosest(const Ray &ray) const
{
	float distance = INFINITY;
	//for each face
	for (int i = 0; i < 12; i++)
	{
		if (fabs(dot(ray.direction, faces[i].normal)) <= SMALL_NUMBER) continue;
		//distance
		float t = dot(faces[i].p1 - ray.origin, faces[i].normal) / dot(ray.direction, faces[i].normal);
		if (t == INFINITY || t < 0.0f) continue;
		//point
		Vector4Df p_plane = ray.origin + ray.direction * t;

		if (faces[i].Contains(p_plane) == true)
		{
			if (t < distance)
				distance = t;
		}
	}
	return distance;
}

RayCollisionList Cube::Collide(const Ray &ray)
{
	//Colisions
	RayCollisionList col;
	Collision P;

	P.geometry = this;

	//for each face
	for (int i = 0; i < 12; i++)
	{
		if (fabs(dot(ray.direction, faces[i].normal)) <= SMALL_NUMBER) continue;
		//distance
		float t = dot(faces[i].p1 - ray.origin, faces[i].normal) / dot(ray.direction, faces[i].normal);
		if (t == INFINITY || t < 0.0f) continue;
		//point
		Vector4Df p_plane = ray.origin + ray.direction * t;

		if (faces[i].Contains(p_plane) == true)
		{
			P.t = t;
			P.point = p_plane;
			col.collisions.push_back(P);
		}

		if (col.collisions.size() == 2) break;
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

Vector4Df Cube::getNormal(const Vector4Df &point)
{
	for (int i = 0; i < 12; i++)
	{
		if (faces[i].Contains(point) == true)
			return faces[i].normal;
	}
}

box Cube::getBB()
{
	box BB;

	BB.point = center - Vector4Df{ sizeX, sizeY, sizeZ, 0.0f };
	BB.size = sizeX;
	if (sizeY > BB.size) BB.size = sizeY;
	if (sizeZ > BB.size) BB.size = sizeZ;

	return BB;
}