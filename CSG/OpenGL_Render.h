#pragma once
#include <iostream>
#include <string>

#include <GL/glut.h>

#include "Sphere.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Cube.h"
#include "CSG.h"

namespace OpenGL_Render
{
	//void DrawObjs(const vector<Object> &objList)
	//{
	//	for (int i = 0; i < objList.size(); i++)
	//	{
	//		std::string geom = typeid(*objList[i].getGeometry()).name(); //get geometry class name
	//		if (geom == "class Sphere")
	//			//std::cout << "DRAW SPHERE" << std::endl;
	//			DrawSphere(objList[i]);
	//		else std::cout << "NOK" << std::endl;
	//	}
	//}

	void DrawCubes(const vector<Cube> &cubeList)
	{
		for (int i = 0; i < cubeList.size(); i++)
		{
			glLineWidth(1.0f);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glPushMatrix();
			glTranslatef((GLfloat)cubeList[i].center.x, (GLfloat)cubeList[i].center.y, (GLfloat)cubeList[i].center.z);
			glScalef((GLfloat)cubeList[i].sizeX, (GLfloat)cubeList[i].sizeY, (GLfloat)cubeList[i].sizeZ);
			glutWireCube(1.0f);
			glPopMatrix();
		}
	}

	void DrawSpheres(const vector<Sphere> &sphereList)
	{
		for (int i = 0; i < sphereList.size(); i++)
		{
			glLineWidth(1.0f);
			//glColor4f(1.0f, 0.5f, 1.0f, 1.0f);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glPushMatrix();
			glTranslatef((GLfloat)sphereList[i].center.x, (GLfloat)sphereList[i].center.y, (GLfloat)sphereList[i].center.z);
			//glutSolidSphere((double)sphereList[i].radius, 50, 50);
			glutWireSphere((double)sphereList[i].radius, 20, 20);
			glPopMatrix();
		}
	}

	void DrawCylinders(const vector<Cylinder> &cylinderList)
	{
		for (int i = 0; i < cylinderList.size(); i++)
		{
			Vector4Df d = cylinderList[i].direction.to_unitary();
			Vector4Df u = cross(d, { 0.0f, 0.0f, 1.0f, 0.0f }).to_unitary();
			float ang = acosf(dot(d, { 0.0f, 0.0f, 1.0f, 0.0f }));
			ang = 180.0f * ang / M_PI;

			glLineWidth(1.0f);
			//glColor4f(1.0f, 0.5f, 1.0f, 1.0f);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glPushMatrix();
			glTranslatef((GLfloat)cylinderList[i].base_center.x, (GLfloat)cylinderList[i].base_center.y, (GLfloat)cylinderList[i].base_center.z);
			glRotatef(-ang, u.x, u.y, u.z);
			//glutSolidCylinder((double)cylinderList[i].radius, (double)cylinderList[i].height, 50, 50);
			glutWireCylinder((double)cylinderList[i].radius, (double)cylinderList[i].height, 20, 20);
			glPopMatrix();
		}
	}

	void DrawCones(const vector<Cone> &coneList)
	{
		for (int i = 0; i < coneList.size(); i++)
		{
			Vector4Df d = coneList[i].direction.to_unitary();
			Vector4Df u = cross(d, { 0.0f, 0.0f, 1.0f, 0.0f }).to_unitary();
			float ang = acosf(dot(d, { 0.0f, 0.0f, 1.0f, 0.0f }));
			ang = 180.0f * ang / M_PI;

			glLineWidth(1.0f);
			//glColor4f(1.0f, 0.5f, 1.0f, 1.0f);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glPushMatrix();
			glTranslatef((GLfloat)coneList[i].base_center.x, (GLfloat)coneList[i].base_center.y, (GLfloat)coneList[i].base_center.z);
			glRotatef(-ang, u.x, u.y, u.z);
			glutWireCone((double)coneList[i].radius, (double)coneList[i].height, 20, 20);
			glPopMatrix();
		}
	}

	void DrawEdges(const vector<Edge> &edgeList)
	{
		for (int i = 0; i < edgeList.size(); i++)
		{
			glLineWidth(3.0f);

			if (edgeList[i].status == 1)
				glColor4f(1.0f, 0.0f, 0.0f, 1.0f); //IN
			else glColor4f(0.0f, 0.0f, 1.0f, 1.0f); //ON or OUT

			glBegin(GL_LINES);
			glVertex3f((GLfloat)edgeList[i].start.x, (GLfloat)edgeList[i].start.y, (GLfloat)edgeList[i].start.z);
			glVertex3f((GLfloat)edgeList[i].end.x, (GLfloat)edgeList[i].end.y, (GLfloat)edgeList[i].end.z);
			glEnd();
		}
	}
}
