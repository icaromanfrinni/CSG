#pragma once
#ifndef OCTREE_H
#define OCTREE_H

#include <string>
#include <vector>
#include "ObjFile.h"
#include "Linear_Algebra.h"
using namespace std;

//------------------------------------------------------------------------------
// TYPES
//------------------------------------------------------------------------------

struct box
{
	vector<box> *childrens;
	box *parent;
	char status; //F: full, E: empty, P: partial
	Vector4Df point;
	float size;
	int level;
	//bool isLeaf;
};

struct ocTree
{
	string name;
	vector<box> tree;
};

struct lastNode
{
	int position;
	int leaf;
};

class Octree
{
public:
	//DEFAULT CONSTRUCTOR
	Octree();
	//CONSTRUCTOR FROM TREE DEEP
	Octree(int d);
	//DESTRUCTOR
	~Octree();

	//VECTOR OF VERTICES FROM VOXEL
	vector<Vector4Df> Vertices(const box &voxel);
	//CENTER FROM VOXEL
	Vector4Df Center(const box &voxel);

	//CHECK if a point 'p' is contained in a voxel
	bool IsContained(const Vector4Df &p, const box &voxel);
	//CHECK if set 'A' is a subset of a set 'B'
	bool IsSubset(const box &A, const box &B);
	//CHECK if a box is equal to another box
	bool IsEqual(const box &A, const box &B);

	/* FUNCTION DECLARATIONS FOR SPECIFIC OBJECT FROM .OBJ */
	box BoundingBox(const obj &object); //CREATE A BOUNDING BOX
	void StateCheck(const obj &object, box &cube); //STATE CHECK
	vector<box> *ObjDivide(const box &pai); //SUBDIVIDE "PARTIAL" BOX
	void Solid(const vector<obj> &objectList); //CREATE THE OCTREE

	/* PRIMITIVES */
	void Box(Vector4Df corner, float sizeX, float sizeY, float sizeZ);
	void Sphere(Vector4Df center, float radius);
	void Cylinder(Vector4Df center, Vector4Df direction, float radius, float height);
	void Cone(Vector4Df center, Vector4Df direction, float radius, float height);
	//vector<box> Divide(const box &dad);
	vector<box> *Divide(const box &dad);

	/* BOOLEAN OPERATIONS */
	box GlobalBB();
	void Union();
	void Subtract();
	void Intersect();

	/* MASS PROPERTY */
	string Volume();
	string Area();

	/* READ TREE FILE */
	bool ReadTreeFile();
	/* WRITE TREE FILE */
	void WriteTreeFile();

	vector<ocTree> treeList;
	bool viewEnable;
	int deep;
};

#endif // OCTREE_H