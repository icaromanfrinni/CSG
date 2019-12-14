#pragma once
#ifndef OBJFILE_H
#define OBJFILE_H

#include <string>
#include <vector>
#include "Linear_Algebra.h"
using namespace std;

//------------------------------------------------------------------------------
// TYPES
//------------------------------------------------------------------------------

struct objLine
{
	Vector4Df vFrom;
	Vector4Df vTo;
};

struct objFace
{
	vector<Vector4Df> vertices;
};

struct obj
{
	string Name;
	vector<Vector4Df> Vertices;
	vector<objLine> Lines;
	vector<objFace> Faces;
};

//------------------------------------------------------------------------------
// CLASSES
//------------------------------------------------------------------------------

class ObjFile
{
public:
	//DEFAULT CONSTRUCTOR
	ObjFile();
	//DESTRUCTOR
	~ObjFile();

	//CLEAR A OBJ VARIABLE TYPE
	void ClearCurrentObj(obj &_obj);
	//READ .OBJ FILE
	bool ReadObjFile();
	//WRITE .OBJ FILE
	void WriteObjFile();

	//ROTATION USING QUATERNIONS
	void Rotation(const Vector4Df &fixedPoint, Vector4Df &axis, float &angle);

	//VARIABLES
	vector<obj> objectList; //vector of objets from OBJ File
	bool viewEnable;
private: 
	vector<Vector4Df> allVertices; /* Create a unique vector of OBJ points */
};

#endif // OBJFILE_H