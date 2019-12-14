#include "Octree.h"
#include "Ray.h"
#include "Triangle.h"
#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <ctime>
using namespace std;

#define DEBUG 0

//DEFAULT CONSTRUCTOR
Octree::Octree()
{
	treeList.clear();
	viewEnable = false;
	deep = 0;
}

//CONSTRUCTOR FROM TREE DEEP
Octree::Octree(int d)
{
	treeList.clear();
	viewEnable = false;
	deep = d;
}

//DESTRUCTOR
Octree::~Octree()
{
}

//VECTOR OF VERTICES FROM VOXEL
vector<Vector4Df> Octree::Vertices(const box &voxel)
{
	vector<Vector4Df> V8;
	V8.push_back(voxel.point);
	V8.push_back({ voxel.point.x + voxel.size, voxel.point.y, voxel.point.z, voxel.point.w });
	V8.push_back({ voxel.point.x + voxel.size, voxel.point.y, voxel.point.z + voxel.size, voxel.point.w });
	V8.push_back({ voxel.point.x, voxel.point.y, voxel.point.z + voxel.size, voxel.point.w });
	V8.push_back({ voxel.point.x, voxel.point.y + voxel.size, voxel.point.z, voxel.point.w });
	V8.push_back({ voxel.point.x + voxel.size, voxel.point.y + voxel.size, voxel.point.z, voxel.point.w });
	V8.push_back({ voxel.point.x + voxel.size, voxel.point.y + voxel.size, voxel.point.z + voxel.size, voxel.point.w });
	V8.push_back({ voxel.point.x, voxel.point.y + voxel.size, voxel.point.z + voxel.size, voxel.point.w });
	return V8;
}

//CENTER FROM VOXEL
Vector4Df Octree::Center(const box &voxel)
{
	return { voxel.point.x + voxel.size / 2.0f, voxel.point.y + voxel.size / 2.0f, voxel.point.z + voxel.size / 2.0f, 1.0f };
}

//CHECK if a point 'p' is contained in a voxel
bool Octree::IsContained(const Vector4Df &p, const box &voxel)
{	
	if (p.x > voxel.point.x && p.x < voxel.point.x + voxel.size && 
		p.y > voxel.point.y && p.y < voxel.point.y + voxel.size &&
		p.z > voxel.point.z && p.z < voxel.point.z + voxel.size)
		return true;

	return false;
}

//CHECK if set 'A' is a subset of a set 'B'
bool Octree::IsSubset(const box &A, const box &B)
{
	if (IsContained(A.point, B) == true)
	{
		if (A.point.x + A.size <= B.point.x + B.size &&
			A.point.y + A.size <= B.point.y + B.size &&
			A.point.z + A.size <= B.point.z + B.size)
			return true;
		else return false;
	}
	else return false;
}

//CHECK if a box is equal to another box
bool Octree::IsEqual(const box &A, const box &B)
{
	if (A.point == B.point && A.size == B.size) return true;
	else return false;
}

/* ===================== * FUNCTION DECLARATIONS FOR .OBJ FILE * ===================== */

//CREATE A BOUNDING BOX OF SPECIFIC OBJECT FROM .OBJ
box Octree::BoundingBox(const obj &object)
{
	//initialize
	box B;
	Vector4Df pMin = { object.Vertices[0].x, object.Vertices[0].y, object.Vertices[0].z, 1.0f };
	Vector4Df pMax = pMin;
	//looking for extreme points
	for (int i = 0; i < object.Vertices.size(); i++)
	{
		if (object.Vertices[i].x < pMin.x) pMin.x = object.Vertices[i].x;
		if (object.Vertices[i].y < pMin.y) pMin.y = object.Vertices[i].y;
		if (object.Vertices[i].z < pMin.z) pMin.z = object.Vertices[i].z;

		if (object.Vertices[i].x > pMax.x) pMax.x = object.Vertices[i].x;
		if (object.Vertices[i].y > pMax.y) pMax.y = object.Vertices[i].y;
		if (object.Vertices[i].z > pMax.z) pMax.z = object.Vertices[i].z;
	}

	//Max difference between extreme points = size of bounding box
	B.size = pMax.x - pMin.x;
	if (pMax.y - pMin.y > B.size)
		B.size = pMax.y - pMin.y;
	if (pMax.z - pMin.z > B.size)
		B.size = pMax.z - pMin.z;

	B.point = pMin; //box point reference
	B.level = 0;
	B.status = 'P';
	B.parent = NULL;

	return B;
}

//STATE CHECK
void Octree::StateCheck(const obj &object, box &cube)
{
	int nodeCount = 0;

	//vetor de vértices do cubo
	vector<Vector4Df> vertexList = Vertices(cube);
	//vertexList.push_back(Center(cube));

	/* MÉTODO GERAL */

	//para cada vértice do cubo
	for (int i = 0; i < vertexList.size(); i++)
	{
		Ray ray;
		ray.origin = vertexList[i];
		ray.direction = { 1.1f, 1.2f, 1.3f, 0.0f };
		ray.direction.normalize();

		int collisionCount = 0;

		if (DEBUG == 1)
		{
			std::cout << "vertex " << i << " = " << "(" << vertexList[i].x << ", " << vertexList[i].y << ", " << vertexList[i].z << ")" << std::endl;
		}
		

		//Vector4Df lastP = { SMALL_NUMBER, SMALL_NUMBER, SMALL_NUMBER, 1.0f };
		//para cada face do obj
		for (int j = 0; j < object.Faces.size(); j++)
		{
			
			if (DEBUG == 1)
			{
				std::cout << "face " << j << std::endl;
			}
			
			//se pelo menos um vértice da face corrente estiver dentro do voxel
			//esse voxel fica parcial
			for (int k = 0; k < object.Faces[j].vertices.size(); k++)
			{
				if (IsContained(object.Faces[j].vertices[k], cube) == true)
				{
					if (cube.level == deep) cube.status = 'F';
					else cube.status = 'P';
					return;
				}
			}
			

			//para cada triângulo da face (para faces com 3 ou mais vértices)
			for (int k = 1; k < object.Faces[j].vertices.size() - 1; k++)
			{
				Triangle face(object.Faces[j].vertices[0], object.Faces[j].vertices[k], object.Faces[j].vertices[k + 1]);

				if (fabs(dot(face.p1 - ray.origin, face.normal)) <= SMALL_NUMBER)
					if (face.Contains(ray.origin) == true)
					{

						collisionCount++;

						if (DEBUG == 1)
						{
							std::cout << "! ray jah esta no triangulo !" << std::endl;
						}

						break;
					}

				//se angulo entre raio e normal da face = 90 -> nunca intersepta
				if (fabs(dot(ray.direction, face.normal)) <= SMALL_NUMBER) break;
				//distância
				float t = dot(face.p1 - ray.origin, face.normal) / dot(ray.direction, face.normal);
				//se distância for INFINITA ou NEGATIVA -> aborta
				if (t == INFINITY || t < 0.0f) break;

				Vector4Df p_plane = ray.origin + ray.direction * t;

				

				if (face.Contains(p_plane) == true)
				{
					//se acerta uma aresta de dois planos consecutivos
					//if (p_plane == lastP) break;

					collisionCount++;
					//lastP = p_plane;
					
					if (DEBUG == 1)
					{
						std::cout << "t " << t << std::endl;
						std::cout << "P no plano = " << "(" << p_plane.x << ", " << p_plane.y << ", " << p_plane.z << ")" << std::endl;
						std::cout << "collision count = " << collisionCount << std::endl;
					}

					break;
				}
			}
		}
		if (collisionCount % 2) nodeCount++; //se ímpar, nó do cubo dentro
	}

	if (nodeCount == 8) cube.status = 'F';
	else if (nodeCount == 0) cube.status = 'E';
	else
	{
		if (cube.level == deep) cube.status = 'F';
		else cube.status = 'P';
	}

	if (DEBUG == 1)
	{
		std::cout << "node count = " << nodeCount << std::endl;
		std::cout << "cube status = " << cube.status << std::endl;
	}
	//system("pause");
}

//SUBDIVIDE "GRAY" BOX
vector<box> *Octree::ObjDivide(const box &dad)
{
	vector<box> *childrens = new vector<box>(8);

	for (int i = 0; i < 8; i++)
	{
		box child;
		child.level = dad.level + 1;
		child.size = dad.size / 2.0f;
		child.point = dad.point;

		//parent
		child.parent = new box;
		child.parent->level = dad.level;
		child.parent->size = dad.size;
		child.parent->childrens = dad.childrens;
		child.parent->parent = dad.parent;
		child.parent->status = dad.status;

		if (i == 0) { child.point.z += child.size; }
		if (i == 1) { child.point.z += child.size; child.point.x += child.size; }
		if (i == 2) { child.point.y += child.size; child.point.z += child.size; child.point.x += child.size; }
		if (i == 3) { child.point.y += child.size; child.point.z += child.size; }

		/* if (i == 4) */
		if (i == 5) { child.point.x += child.size; }
		if (i == 6) { child.point.y += child.size; child.point.x += child.size; }
		if (i == 7) { child.point.y += child.size; }

		/*if (son.status == "F" || son.status == "E") son.leaf = true;
		else if (son.level == deep) son.leaf = true;*/

		//adicionar filho
		childrens->at(i) = child;
	}

	return childrens;
}

//CREATE THE OCTREE OF SPECIFIC OBJECT FROM .OBJ
void Octree::Solid(const vector<obj> &objectList)
{
	//Initialize
	treeList.clear();
	clock_t begin = clock();

	for (int i = 0; i < objectList.size(); i++)
	{
		ocTree currentObj;
		//bounding box
		box BB = BoundingBox(objectList[i]);
		currentObj.tree.push_back(BB);

		//dividir pela profundidade da árvore
		for (int j = 0; j < currentObj.tree.size(); j++)
			if (currentObj.tree[j].level < deep && currentObj.tree[j].status == 'P')
			{
				vector<box> *kids = new vector<box>(8);
				kids = Divide(currentObj.tree[j]);

				// State Check
				for (int k = 0; k < 8; k++)
				{
					if (DEBUG == 1)
					{
						std::cout << "box " << k << std::endl;
					}

					/*kids->at(k).status = */StateCheck(objectList[i], kids->at(k));
				}

				currentObj.tree[i].childrens = kids;
				// Add filhos
				currentObj.tree.insert(currentObj.tree.end(), kids->begin(), kids->end());

				if (DEBUG == 1)
				{
					std::cout << "size = " << currentObj.tree.size() << std::endl;
				}
				//system("pause");
			}		

		std::cout << objectList[i].Name << std::endl;
		clock_t end = clock();
		float elapsed_secs = float(end - begin) / CLOCKS_PER_SEC;
		std::cout << "Elapsed secs = " << elapsed_secs << std::endl;

		treeList.push_back(currentObj);
	}
}

/* ===================== * FUNCTION DECLARATIONS FOR PRIMITIVES * ===================== */

vector<box> *Octree::Divide(const box &dad)
{
	vector<box> *childrens = new vector<box>(8);

	for (int i = 0; i < 8; i++)
	{
		box child;
		child.level = dad.level + 1;
		child.size = dad.size / 2.0f;
		child.point = dad.point;

		//parent
		child.parent = new box;
		child.parent->level = dad.level;
		child.parent->size = dad.size;
		child.parent->childrens = dad.childrens;
		child.parent->parent = dad.parent;
		child.parent->status = dad.status;

		if (i == 0) { child.point.z += child.size; }
		if (i == 1) { child.point.z += child.size; child.point.x += child.size; }
		if (i == 2) { child.point.y += child.size; child.point.z += child.size; child.point.x += child.size; }
		if (i == 3) { child.point.y += child.size; child.point.z += child.size; }

		/* if (i == 4) */
		if (i == 5) { child.point.x += child.size; }
		if (i == 6) { child.point.y += child.size; child.point.x += child.size; }
		if (i == 7) { child.point.y += child.size; }

		//adicionar filho
		childrens->at(i) = child;
	}

	return childrens;
}

void Octree::Sphere(Vector4Df center, float radius)
{
	//INITIALIZE
	ocTree currentTree;
	currentTree.name = "Sphere";
	//bounding box
	box BB;
	BB.size = 2.0f * radius;
	BB.level = 0;
	BB.status = 'P';
	BB.point = { center.x - radius, center.y - radius, center.z - radius, 1.0f };
	BB.parent = NULL;
	currentTree.tree.push_back(BB);
	//dividir pela profundidade da árvore
	for (int i = 0; i < currentTree.tree.size(); i++)
	{
		if (currentTree.tree[i].level < deep && currentTree.tree[i].status == 'P')
		{
			vector<box> *kids = new vector<box>(8);
			kids = Divide(currentTree.tree[i]);

			// State Check
			for (int j = 0; j < 8; j++)
			{
				//vetor de vértices do cubo
				vector<Vector4Df> nodes = Vertices(kids->at(j));
				//vértices extremos
				Vector4Df wMin = nodes[0] - center;
				Vector4Df wMax = nodes[0] - center;
				for (int k = 1; k < 8; k++)
				{
					if ((nodes[k] - center).length() < wMin.length()) wMin = nodes[k] - center;
					if ((nodes[k] - center).length() > wMax.length()) wMax = nodes[k] - center;
				}
				if (wMin.length() >= radius) kids->at(j).status = 'E';
				else if (wMax.length() <= radius) kids->at(j).status = 'F';
				else kids->at(j).status = 'P';

				//pega as parciais q são "folhas", e verifica o quanto ela é "full" ou "empty" de acordo com o centro o cubo
				if (kids->at(j).status == 'P' && kids->at(j).level == deep)
				{
					Vector4Df w = { kids->at(j).point.x + kids->at(j).size / 2.0f, kids->at(j).point.y + kids->at(j).size / 2.0f, kids->at(j).point.z + kids->at(j).size / 2.0f, kids->at(j).point.w };
					w -= center;
					if (w.length() >= radius) kids->at(j).status = 'E';
					else if (w.length() <= radius) kids->at(j).status = 'F';
				}
			}
			currentTree.tree[i].childrens = kids;
			// Add filhos
			currentTree.tree.insert(currentTree.tree.end(), kids->begin(), kids->end());
		}
	}

	treeList.push_back(currentTree);

	/* DEBUG */
	std::cout << "========== SPHERE ==========" << std::endl;
	std::cout << "Area = " << 4.0f * M_PI * powf(radius, 2.0f) << std::endl;
	std::cout << "Volume = " << (4.0f / 3.0f) * M_PI * powf(radius, 3.0f) << std::endl;
}

void Octree::Cylinder(Vector4Df center, Vector4Df direction, float radius, float height)
{
	//INITIALIZE
	direction.normalize();
	ocTree currentTree;
	currentTree.name = "Cylinder";
	//bounding box
	box BB;
	if (height > 2.0f * radius) BB.size = height;
	else BB.size = 2.0f * radius;
	BB.level = 0;
	BB.status = 'P';
	BB.point = { center.x - BB.size / 2.0f, center.y, center.z - BB.size / 2.0f, 1.0f };
	BB.parent = NULL;
	currentTree.tree.push_back(BB);
	//dividir pela profundidade da árvore
	for (int i = 0; i < currentTree.tree.size(); i++)
	{
		if (currentTree.tree[i].level < deep && currentTree.tree[i].status == 'P')
		{
			vector<box> *kids = new vector<box>(8);
			kids = Divide(currentTree.tree[i]);

			// State Check
			for (int j = 0; j < 8; j++)
			{
				//vetor de vértices do cubo
				vector<Vector4Df> nodes = Vertices(kids->at(j));
				//vértices extremos
				Vector4Df wMin = nodes[0] - center;
				Vector4Df wMax = nodes[0] - center;
				for (int k = 1; k < 8; k++)
				{
					if ((nodes[k] - center).length() < wMin.length()) wMin = nodes[k] - center;
					if ((nodes[k] - center).length() > wMax.length()) wMax = nodes[k] - center;
				}
				Vector4Df rMin = wMin - direction * dot(wMin, direction);
				Vector4Df rMax = wMax - direction * dot(wMax, direction);
				if (rMin.length() >= radius)  kids->at(j).status = 'E';
				else if (rMax.length() <= radius)  kids->at(j).status = 'F';
				else  kids->at(j).status = 'P';

				//pega as parciais q são "folhas", e verifica o quanto ela é "full" ou "empty" de acordo com o centro o cubo
				if ( kids->at(j).status == 'P' &&  kids->at(j).level == deep)
				{
					Vector4Df w = {  kids->at(j).point.x +  kids->at(j).size / 2.0f,  kids->at(j).point.y +  kids->at(j).size / 2.0f,  kids->at(j).point.z +  kids->at(j).size / 2.0f,  kids->at(j).point.w };
					w -= center;
					Vector4Df r = w - direction * dot(w, direction);
					if (r.length() >= radius)  kids->at(j).status = 'E';
					else if (r.length() <= radius)  kids->at(j).status = 'F';
				}
			}
			currentTree.tree[i].childrens = kids;
			// Add filhos
			currentTree.tree.insert(currentTree.tree.end(), kids->begin(), kids->end());
		}
	}

	treeList.push_back(currentTree);

	/* DEBUG */
	std::cout << "========== CYLINDER ==========" << std::endl;
	std::cout << "Area = " << 2.0f * M_PI * radius * (height + radius) << std::endl;
	std::cout << "Volume = " << M_PI * powf(radius, 2.0f) * height << std::endl;
}

void Octree::Box(Vector4Df corner, float sizeX, float sizeY, float sizeZ)
{
	//INITIALIZE
	ocTree currentTree;
	currentTree.name = "Box";
	//bounding box
	box BB;
	BB.size = sizeX;
	if (sizeY > BB.size) BB.size = sizeY;
	if (sizeZ > BB.size) BB.size = sizeZ;
	BB.level = 0;
	BB.status = 'P';
	BB.point = corner;
	BB.parent = NULL;
	currentTree.tree.push_back(BB);
	//dividir pela profundidade da árvore
	for (int i = 0; i < currentTree.tree.size(); i++)
	{
		if (currentTree.tree[i].level < deep && currentTree.tree[i].status == 'P')
		{
			vector<box> *kids = new vector<box>(8);
			kids = Divide(currentTree.tree[i]);

			// State Check
			for (int j = 0; j < 8; j++)
			{
				//vetor de vértices do cubo
				vector<Vector4Df> nodes = Vertices(kids->at(j));
				//vértices extremos
				Vector4Df wMin = nodes[0] - corner;
				Vector4Df wMax = nodes[0] - corner;
				for (int k = 1; k < 8; k++)
				{
					if ((nodes[k] - corner).length() < wMin.length()) wMin = nodes[k] - corner;
					if ((nodes[k] - corner).length() > wMax.length()) wMax = nodes[k] - corner;
				}

				if (fabs(wMin.x) >= sizeX || fabs(wMin.y) >= sizeY || fabs(wMin.z) >= sizeZ) kids->at(j).status = 'E';
				else if (fabs(wMax.x) <= sizeX && fabs(wMax.y) <= sizeY && fabs(wMax.z) <= sizeZ) kids->at(j).status = 'F';
				else kids->at(j).status = 'P';

				//pega as parciais q são "folhas", e verifica o quanto ela é "full" ou "empty" de acordo com o centro o cubo
				if (kids->at(j).status == 'P' && kids->at(j).level == deep)
				{
					Vector4Df w = { kids->at(j).point.x + kids->at(j).size / 2.0f, kids->at(j).point.y + kids->at(j).size / 2.0f, kids->at(j).point.z + kids->at(j).size / 2.0f, kids->at(j).point.w };
					w -= corner;
					if (fabs(w.x) >= sizeX || fabs(w.y) >= sizeY || fabs(w.z) >= sizeZ) kids->at(j).status = 'E';
					else if (fabs(w.x) <= sizeX && fabs(w.y) <= sizeY && fabs(w.z) <= sizeZ) kids->at(j).status = 'F';
				}
			}
			currentTree.tree[i].childrens = kids;
			// Add filhos
			currentTree.tree.insert(currentTree.tree.end(), kids->begin(), kids->end());
		}
	}

	treeList.push_back(currentTree);

	/* DEBUG */
	std::cout << "========== BOX ==========" << std::endl;
	std::cout << "Area = " << 2.0f * (sizeX * sizeY + sizeX * sizeZ + sizeY * sizeZ) << std::endl;
	std::cout << "Volume = " << sizeX * sizeY * sizeZ << std::endl;
}

void Octree::Cone(Vector4Df center, Vector4Df direction, float radius, float height)
{
	//INITIALIZE
	direction.normalize();
	ocTree currentTree;
	currentTree.name = "Cone";
	float cosTeta = height / (sqrtf(powf(height, 2.0f) + powf(radius, 2.0f)));
	//bounding box
	box BB;
	if (height > 2.0f * radius) BB.size = height;
	else BB.size = 2.0f * radius;
	BB.level = 0;
	BB.status = 'P';
	BB.point = { center.x - BB.size / 2.0f, center.y, center.z - BB.size / 2.0f, 1.0f };
	BB.parent = NULL;
	currentTree.tree.push_back(BB);
	//dividir pela profundidade da árvore
	for (int i = 0; i < currentTree.tree.size(); i++)
	{
		if (currentTree.tree[i].level < deep && currentTree.tree[i].status == 'P')
		{
			vector<box> *kids = new vector<box>(8);
			kids = Divide(currentTree.tree[i]);

			// State Check
			for (int j = 0; j < 8; j++)
			{
				//vetor de vértices do cubo
				vector<Vector4Df> nodes = Vertices(kids->at(j));
				//vértices extremos
				Vector4Df wMin = nodes[0] - center;
				Vector4Df wMax = nodes[0] - center;
				for (int k = 1; k < 8; k++)
				{
					if ((nodes[k] - center).length() < wMin.length()) wMin = nodes[k] - center;
					if ((nodes[k] - center).length() > wMax.length()) wMax = nodes[k] - center;
				}
				Vector4Df gMin = (direction * height) - wMin; gMin.normalize();
				Vector4Df gMax = (direction * height) - wMax; gMax.normalize();

				if (dot(gMin, direction) <= cosTeta) kids->at(j).status = 'E';
				else if (dot(gMax, direction) >= cosTeta) kids->at(j).status = 'F';
				else kids->at(j).status = 'P';

				//pega as parciais q são "folhas", e verifica o quanto ela é "full" ou "empty" de acordo com o centro o cubo
				if (kids->at(j).status == 'P' && kids->at(j).level == deep)
				{
					Vector4Df w = { kids->at(j).point.x + kids->at(j).size / 2.0f, kids->at(j).point.y + kids->at(j).size / 2.0f, kids->at(j).point.z + kids->at(j).size / 2.0f, kids->at(j).point.w };
					w -= center;
					Vector4Df g = (direction * height) - w; g.normalize();
					//kids->at(j).status = "F";
					if (dot(g, direction) <= cosTeta) kids->at(j).status = 'E';
					else if (dot(g, direction) >= cosTeta) kids->at(j).status = 'F';
				}
			}
			currentTree.tree[i].childrens = kids;
			// Add filhos
			currentTree.tree.insert(currentTree.tree.end(), kids->begin(), kids->end());
		}
	}

	treeList.push_back(currentTree);

	/* DEBUG */
	std::cout << "========== CONE ==========" << std::endl;
	std::cout << "Area = " << M_PI * radius * (radius + sqrtf(powf(radius, 2.0f) + powf(height, 2.0f))) << std::endl;
	std::cout << "Volume = " << (1.0f / 3.0f) * M_PI * powf(radius, 2.0f) * height << std::endl;
}

/* ===================== * BOOLEAN OPERATIONS * ===================== */

box Octree::GlobalBB()
{
	//Initialize
	box BB;
	Vector4Df pMin = treeList[0].tree[0].point;
	Vector4Df pMax = pMin;

	//percorrer as duas primeiras "árvores"
	for (int i = 0; i < 2; i++)
	{
		//ponto mínimo
		if (treeList[i].tree[0].point.x < pMin.x) pMin.x = treeList[i].tree[0].point.x;
		if (treeList[i].tree[0].point.y < pMin.y) pMin.y = treeList[i].tree[0].point.y;
		if (treeList[i].tree[0].point.z < pMin.z) pMin.z = treeList[i].tree[0].point.z;
		//ponto máximo
		if (treeList[i].tree[0].point.x + treeList[i].tree[0].size > pMax.x) pMax.x = treeList[i].tree[0].point.x + treeList[i].tree[0].size;
		if (treeList[i].tree[0].point.y + treeList[i].tree[0].size > pMax.y) pMax.y = treeList[i].tree[0].point.y + treeList[i].tree[0].size;
		if (treeList[i].tree[0].point.z + treeList[i].tree[0].size > pMax.z) pMax.z = treeList[i].tree[0].point.z + treeList[i].tree[0].size;
	}

	//Max difference between extreme points = size of bounding box
	BB.size = pMax.x - pMin.x;
	if (pMax.y - pMin.y > BB.size)
		BB.size = pMax.y - pMin.y;
	if (pMax.z - pMin.z > BB.size)
		BB.size = pMax.z - pMin.z;

	BB.point = pMin; //box point reference
	BB.level = 0;
	BB.status = 'P';
	BB.parent = NULL;
	return BB;
}

void Octree::Union()
{
	//INITIALIZE
	box BB = GlobalBB();
	ocTree tree1, tree2, unionTree;
	tree1.name = "Tree 1"; tree2.name = "Tree 2"; unionTree.name = "UNION";
	tree1.tree.push_back(BB); tree2.tree.push_back(BB); unionTree.tree.push_back(BB);

	//TREE 1
	for (int i = 0; i < tree1.tree.size(); i++)
	{
		if (tree1.tree[i].level < deep && tree1.tree[i].status == 'P')
		{
			vector<box> *kids = new vector<box>(8);
			kids = Divide(tree1.tree[i]);

			// State Check
			for (int j = 0; j < 8; j++)
			{
				//default
				kids->at(j).status = 'E';
				// varrer cada voxel "full" da octree original
				for (int k = 1; k < treeList[0].tree.size(); k++)
				{
					// apenas os voxels "full"
					if (treeList[0].tree[k].status == 'F')
					{
						// se o filho for igual ao voxel atual -> ele vira full também
						if (IsEqual(kids->at(j), treeList[0].tree[k]) == true ||
							IsSubset(kids->at(j), treeList[0].tree[k]) == true) {
							kids->at(j).status = 'F'; break;
						}
						else
						{
							// se o voxel estiver contido no filho -> filho vira parcial
							// mas se for folha -> filho vira full
							if (IsSubset(treeList[0].tree[k], kids->at(j)) == true)
							{
								if (kids->at(j).level == deep) { kids->at(j).status = 'F'; break; }
								else { kids->at(j).status = 'P'; break; }
							}
							else
							{
								// se algum vértice do voxel full estiver contido no filho -> filho vira parcial
								// mas se for folha -> filho vira full
								vector<Vector4Df> voxelVertices = Vertices(treeList[0].tree[k]);
								int nodeCount = 0;
								for (int n = 0; n < 8; n++)
									if (IsContained(voxelVertices[n], kids->at(j)) == true) nodeCount++;
								if (nodeCount > 0)
								{
									//std::cout << "nodeCount = " << nodeCount << std::endl;
									if (kids->at(j).level == deep) { kids->at(j).status = 'F'; break; }
									else { kids->at(j).status = 'P'; break; }
								}
								else
								{
									// se algum vértice do filho parcial estiver contido no voxel full -> filho vira parcial
									// mas se for folha -> filho vira full
									voxelVertices = Vertices(kids->at(j));
									nodeCount = 0;
									for (int n = 0; n < 8; n++)
										if (IsContained(voxelVertices[n], treeList[0].tree[k]) == true) nodeCount++;
									if (nodeCount > 0)
									{
										//std::cout << "nodeCount2 = " << nodeCount << std::endl;
										if (kids->at(j).level == deep) { kids->at(j).status = 'F'; break; }
										else { kids->at(j).status = 'P'; break; }
									}
								}
							}
						}
					}
				}
			}
			tree1.tree[i].childrens = kids;
			// Add filhos
			tree1.tree.insert(tree1.tree.end(), kids->begin(), kids->end());
		}
	}

	//TREE 2
	for (int i = 0; i < tree2.tree.size(); i++)
	{
		if (tree2.tree[i].level < deep && tree2.tree[i].status == 'P')
		{
			vector<box> *kids = new vector<box>(8);
			kids = Divide(tree2.tree[i]);

			// State Check
			for (int j = 0; j < 8; j++)
			{
				//default
				kids->at(j).status = 'E';
				// varrer cada voxel "full" da octree original
				for (int k = 1; k < treeList[1].tree.size(); k++)
				{
					// apenas os voxels "full"
					if (treeList[1].tree[k].status == 'F')
					{
						// se o filho for igual ao voxel atual -> ele vira full também
						if (IsEqual(kids->at(j), treeList[1].tree[k]) == true ||
							IsSubset(kids->at(j), treeList[1].tree[k]) == true) {
							kids->at(j).status = 'F'; break;
						}
						else
						{
							// se o voxel estiver contido no filho -> filho vira parcial
							// mas se for folha -> filho vira full
							if (IsSubset(treeList[1].tree[k], kids->at(j)) == true)
							{
								if (kids->at(j).level == deep) { kids->at(j).status = 'F'; break; }
								else { kids->at(j).status = 'P'; break; }
							}
							else
							{
								// se algum vértice do voxel full estiver contido no filho -> filho vira parcial
								// mas se for folha -> filho vira full
								vector<Vector4Df> voxelVertices = Vertices(treeList[1].tree[k]);
								int nodeCount = 0;
								for (int n = 0; n < 8; n++)
									if (IsContained(voxelVertices[n], kids->at(j)) == true) nodeCount++;
								if (nodeCount > 0)
								{
									//std::cout << "nodeCount = " << nodeCount << std::endl;
									if (kids->at(j).level == deep) { kids->at(j).status = 'F'; break; }
									else { kids->at(j).status = 'P'; break; }
								}
								else
								{
									// se algum vértice do filho parcial estiver contido no voxel full -> filho vira parcial
									// mas se for folha -> filho vira full
									voxelVertices = Vertices(kids->at(j));
									nodeCount = 0;
									for (int n = 0; n < 8; n++)
										if (IsContained(voxelVertices[n], treeList[1].tree[k]) == true) nodeCount++;
									if (nodeCount > 0)
									{
										//std::cout << "nodeCount2 = " << nodeCount << std::endl;
										if (kids->at(j).level == deep) { kids->at(j).status = 'F'; break; }
										else { kids->at(j).status = 'P'; break; }
									}
								}
							}
						}
					}
				}
			}
			tree2.tree[i].childrens = kids;
			// Add filhos
			tree2.tree.insert(tree2.tree.end(), kids->begin(), kids->end());
		}
	}

	//Initialize the UNION TREE
	for (int i = 0; i < unionTree.tree.size(); i++)
	{
		if (unionTree.tree[i].level < deep && unionTree.tree[i].status == 'P')
		{
			vector<box> *kids = new vector<box>(8);
			kids = Divide(unionTree.tree[i]);

			// State Check
			for (int j = 0; j < 8; j++)
			{
				if (kids->at(j).level == deep) kids->at(j).status = 'E';
				else kids->at(j).status = 'P';
			}
			unionTree.tree[i].childrens = kids;
			// Add filhos
			unionTree.tree.insert(unionTree.tree.end(), kids->begin(), kids->end());
		}
	}

	//UNION
	for (int i = 1; i < unionTree.tree.size(); i++)
	{
		//default
		box A = unionTree.tree[i], B = unionTree.tree[i];
		//se achar voxel igual em 1, pega!
		for (int j = 1; j < tree1.tree.size(); j++)
			if (tree1.tree[j].level == unionTree.tree[i].level && tree1.tree[j].point == unionTree.tree[i].point)
				A = tree1.tree[j];
		////se achar voxel igual em 2, pega!
		for (int j = 1; j < tree2.tree.size(); j++)
			if (tree2.tree[j].level == unionTree.tree[i].level && tree2.tree[j].point == unionTree.tree[i].point)
				B = tree2.tree[j];

		//test
		if (A.status == 'F' || B.status == 'F') unionTree.tree[i].status = 'F';
	}

	/* ================================================= */
	/* PERFORM SET OPERATIONS BETWEEN TWO LINEAR OCTREES */
	/* ================================================= */
	//Initialize
	//ocTree Union, Intersection, Diff12, Diff21;
	//Union.name = "UNION"; Intersection.name = "INTERSECTION"; Diff12.name = "1 MINUS 2"; Diff21.name = "2 MINUS 1";
	//Union.tree.push_back(BB); Intersection.tree.push_back(BB); Diff12.tree.push_back(BB); Diff21.tree.push_back(BB);
	//int Int_ptr = 0, Union_ptr = 0, Diff12_ptr = 0, Diff21_ptr = 0;
	//int Max = max(tree1.tree.size(), tree2.tree.size());
	//int Ptr1 = 1, Ptr2 = 1;
	//while (Ptr1 <= tree1.tree.size() && Ptr2 <= tree2.tree.size())
	//{
	//	if (IsSubset(tree1.tree[Ptr1], tree2.tree[Ptr2]) == true) //tree1 C tree2
	//	{
	//		//Intersection
	//		Int_ptr++;
	//		Intersection.tree[Int_ptr] = tree1.tree[Ptr1];
	//		//Difference
	//		if 
	//	}
	//}

	//CLOSE
	treeList.clear();
	treeList.push_back(unionTree);
	/*treeList.push_back(tree1);
	treeList.push_back(tree2);*/
}

void Octree::Subtract()
{
	//INITIALIZE
	box BB = GlobalBB();
	ocTree tree1, tree2, subTree;
	tree1.name = "Tree 1"; tree2.name = "Tree 2"; subTree.name = "SUBTRACT";
	tree1.tree.push_back(BB); tree2.tree.push_back(BB); subTree.tree.push_back(BB);

	//TREE 1
	for (int i = 0; i < tree1.tree.size(); i++)
	{
		if (tree1.tree[i].level < deep && tree1.tree[i].status == 'P')
		{
			vector<box> *kids = new vector<box>(8);
			kids = Divide(tree1.tree[i]);

			// State Check
			for (int j = 0; j < 8; j++)
			{
				//default
				kids->at(j).status = 'E';
				// varrer cada voxel "full" da octree original
				for (int k = 1; k < treeList[0].tree.size(); k++)
				{
					// apenas os voxels "full"
					if (treeList[0].tree[k].status == 'F')
					{
						// se o filho for igual ao voxel atual -> ele vira full também
						if (IsEqual(kids->at(j), treeList[0].tree[k]) == true ||
							IsSubset(kids->at(j), treeList[0].tree[k]) == true) {
							kids->at(j).status = 'F'; break;
						}
						else
						{
							// se o voxel estiver contido no filho -> filho vira parcial
							// mas se for folha -> filho vira full
							if (IsSubset(treeList[0].tree[k], kids->at(j)) == true)
							{
								if (kids->at(j).level == deep) { kids->at(j).status = 'F'; break; }
								else { kids->at(j).status = 'P'; break; }
							}
							else
							{
								// se algum vértice do voxel full estiver contido no filho -> filho vira parcial
								// mas se for folha -> filho vira full
								vector<Vector4Df> voxelVertices = Vertices(treeList[0].tree[k]);
								int nodeCount = 0;
								for (int n = 0; n < 8; n++)
									if (IsContained(voxelVertices[n], kids->at(j)) == true) nodeCount++;
								if (nodeCount > 0)
								{
									//std::cout << "nodeCount = " << nodeCount << std::endl;
									if (kids->at(j).level == deep) { kids->at(j).status = 'F'; break; }
									else { kids->at(j).status = 'P'; break; }
								}
								else
								{
									// se algum vértice do filho parcial estiver contido no voxel full -> filho vira parcial
									// mas se for folha -> filho vira full
									voxelVertices = Vertices(kids->at(j));
									nodeCount = 0;
									for (int n = 0; n < 8; n++)
										if (IsContained(voxelVertices[n], treeList[0].tree[k]) == true) nodeCount++;
									if (nodeCount > 0)
									{
										//std::cout << "nodeCount2 = " << nodeCount << std::endl;
										if (kids->at(j).level == deep) { kids->at(j).status = 'F'; break; }
										else { kids->at(j).status = 'P'; break; }
									}
								}
							}
						}
					}
				}
			}
			tree1.tree[i].childrens = kids;
			// Add filhos
			tree1.tree.insert(tree1.tree.end(), kids->begin(), kids->end());
		}
	}

	//TREE 2
	for (int i = 0; i < tree2.tree.size(); i++)
	{
		if (tree2.tree[i].level < deep && tree2.tree[i].status == 'P')
		{
			vector<box> *kids = new vector<box>(8);
			kids = Divide(tree2.tree[i]);

			// State Check
			for (int j = 0; j < 8; j++)
			{
				//default
				kids->at(j).status = 'E';
				// varrer cada voxel "full" da octree original
				for (int k = 1; k < treeList[1].tree.size(); k++)
				{
					// apenas os voxels "full"
					if (treeList[1].tree[k].status == 'F')
					{
						// se o filho for igual ao voxel atual -> ele vira full também
						if (IsEqual(kids->at(j), treeList[1].tree[k]) == true ||
							IsSubset(kids->at(j), treeList[1].tree[k]) == true) {
							kids->at(j).status = 'F'; break;
						}
						else
						{
							// se o voxel estiver contido no filho -> filho vira parcial
							// mas se for folha -> filho vira full
							if (IsSubset(treeList[1].tree[k], kids->at(j)) == true)
							{
								if (kids->at(j).level == deep) { kids->at(j).status = 'F'; break; }
								else { kids->at(j).status = 'P'; break; }
							}
							else
							{
								// se algum vértice do voxel full estiver contido no filho -> filho vira parcial
								// mas se for folha -> filho vira full
								vector<Vector4Df> voxelVertices = Vertices(treeList[1].tree[k]);
								int nodeCount = 0;
								for (int n = 0; n < 8; n++)
									if (IsContained(voxelVertices[n], kids->at(j)) == true) nodeCount++;
								if (nodeCount > 0)
								{
									//std::cout << "nodeCount = " << nodeCount << std::endl;
									if (kids->at(j).level == deep) { kids->at(j).status = 'F'; break; }
									else { kids->at(j).status = 'P'; break; }
								}
								else
								{
									// se algum vértice do filho parcial estiver contido no voxel full -> filho vira parcial
									// mas se for folha -> filho vira full
									voxelVertices = Vertices(kids->at(j));
									nodeCount = 0;
									for (int n = 0; n < 8; n++)
										if (IsContained(voxelVertices[n], treeList[1].tree[k]) == true) nodeCount++;
									if (nodeCount > 0)
									{
										//std::cout << "nodeCount2 = " << nodeCount << std::endl;
										if (kids->at(j).level == deep) { kids->at(j).status = 'F'; break; }
										else { kids->at(j).status = 'P'; break; }
									}
								}
							}
						}
					}
				}
			}
			tree2.tree[i].childrens = kids;
			// Add filhos
			tree2.tree.insert(tree2.tree.end(), kids->begin(), kids->end());
		}
	}

	//Initialize the SUBTRACT TREE
	for (int i = 0; i < subTree.tree.size(); i++)
	{
		if (subTree.tree[i].level < deep && subTree.tree[i].status == 'P')
		{
			vector<box> *kids = new vector<box>(8);
			kids = Divide(subTree.tree[i]);

			// State Check
			for (int j = 0; j < 8; j++)
			{
				if (kids->at(j).level == deep) kids->at(j).status = 'E';
				else kids->at(j).status = 'P';
			}
			subTree.tree[i].childrens = kids;
			// Add filhos
			subTree.tree.insert(subTree.tree.end(), kids->begin(), kids->end());
		}
	}

	//SUBTRACT
	for (int i = 1; i < subTree.tree.size(); i++)
	{
		//default
		box A, B;
		A.status = 'E'; B.status = 'E';
		//box A = subTree.tree[i], B = subTree.tree[i];
		//se achar voxel igual em 1, pega!
		for (int j = 1; j < tree1.tree.size(); j++)
			if (tree1.tree[j].level == subTree.tree[i].level && tree1.tree[j].point == subTree.tree[i].point)
				A = tree1.tree[j];
		////se achar voxel igual em 2, pega!
		for (int j = 1; j < tree2.tree.size(); j++)
			if (tree2.tree[j].level == subTree.tree[i].level && tree2.tree[j].point == subTree.tree[i].point)
				B = tree2.tree[j];

		//test
		if (A.status == 'F' && B.status == 'F') subTree.tree[i].status = 'E';
		if (A.status == 'F' && B.status == 'E') subTree.tree[i].status = 'F';
		if (A.status == 'E' && B.status == 'F') subTree.tree[i].status = 'E';
		if (A.status == 'F' && B.status == 'P') subTree.tree[i].status = 'P';
		if (A.status == 'P' && B.status == 'F') subTree.tree[i].status = 'E';
		if (A.status == 'P' && B.status == 'P') subTree.tree[i].status = 'P';
	}

	//CLOSE
	treeList.clear();
	treeList.push_back(subTree);
	/*treeList.push_back(tree1);
	treeList.push_back(tree2);*/
}

void Octree::Intersect()
{
	//INITIALIZE
	box BB = GlobalBB();
	ocTree tree1, tree2, interTree;
	tree1.name = "Tree 1"; tree2.name = "Tree 2"; interTree.name = "INTERSECT";
	tree1.tree.push_back(BB); tree2.tree.push_back(BB); interTree.tree.push_back(BB);

	//TREE 1
	for (int i = 0; i < tree1.tree.size(); i++)
	{
		if (tree1.tree[i].level < deep && tree1.tree[i].status == 'P')
		{
			vector<box> *kids = new vector<box>(8);
			kids = Divide(tree1.tree[i]);

			// State Check
			for (int j = 0; j < 8; j++)
			{
				//default
				kids->at(j).status = 'E';
				// varrer cada voxel "full" da octree original
				for (int k = 1; k < treeList[0].tree.size(); k++)
				{
					// apenas os voxels "full"
					if (treeList[0].tree[k].status == 'F')
					{
						// se o filho for igual ao voxel atual -> ele vira full também
						if (IsEqual(kids->at(j), treeList[0].tree[k]) == true ||
							IsSubset(kids->at(j), treeList[0].tree[k]) == true) {
							kids->at(j).status = 'F'; break;
						}
						else
						{
							// se o voxel estiver contido no filho -> filho vira parcial
							// mas se for folha -> filho vira full
							if (IsSubset(treeList[0].tree[k], kids->at(j)) == true)
							{
								if (kids->at(j).level == deep) { kids->at(j).status = 'F'; break; }
								else { kids->at(j).status = 'P'; break; }
							}
							else
							{
								// se algum vértice do voxel full estiver contido no filho -> filho vira parcial
								// mas se for folha -> filho vira full
								vector<Vector4Df> voxelVertices = Vertices(treeList[0].tree[k]);
								int nodeCount = 0;
								for (int n = 0; n < 8; n++)
									if (IsContained(voxelVertices[n], kids->at(j)) == true) nodeCount++;
								if (nodeCount > 0)
								{
									//std::cout << "nodeCount = " << nodeCount << std::endl;
									if (kids->at(j).level == deep) { kids->at(j).status = 'F'; break; }
									else { kids->at(j).status = 'P'; break; }
								}
								else
								{
									// se algum vértice do filho parcial estiver contido no voxel full -> filho vira parcial
									// mas se for folha -> filho vira full
									voxelVertices = Vertices(kids->at(j));
									nodeCount = 0;
									for (int n = 0; n < 8; n++)
										if (IsContained(voxelVertices[n], treeList[0].tree[k]) == true) nodeCount++;
									if (nodeCount > 0)
									{
										//std::cout << "nodeCount2 = " << nodeCount << std::endl;
										if (kids->at(j).level == deep) { kids->at(j).status = 'F'; break; }
										else { kids->at(j).status = 'P'; break; }
									}
								}
							}
						}
					}
				}
			}
			tree1.tree[i].childrens = kids;
			// Add filhos
			tree1.tree.insert(tree1.tree.end(), kids->begin(), kids->end());
		}
	}

	//TREE 2
	for (int i = 0; i < tree2.tree.size(); i++)
	{
		if (tree2.tree[i].level < deep && tree2.tree[i].status == 'P')
		{
			vector<box> *kids = new vector<box>(8);
			kids = Divide(tree2.tree[i]);

			// State Check
			for (int j = 0; j < 8; j++)
			{
				//default
				kids->at(j).status = 'E';
				// varrer cada voxel "full" da octree original
				for (int k = 1; k < treeList[1].tree.size(); k++)
				{
					// apenas os voxels "full"
					if (treeList[1].tree[k].status == 'F')
					{
						// se o filho for igual ao voxel atual -> ele vira full também
						if (IsEqual(kids->at(j), treeList[1].tree[k]) == true ||
							IsSubset(kids->at(j), treeList[1].tree[k]) == true) {
							kids->at(j).status = 'F'; break;
						}
						else
						{
							// se o voxel estiver contido no filho -> filho vira parcial
							// mas se for folha -> filho vira full
							if (IsSubset(treeList[1].tree[k], kids->at(j)) == true)
							{
								if (kids->at(j).level == deep) { kids->at(j).status = 'F'; break; }
								else { kids->at(j).status = 'P'; break; }
							}
							else
							{
								// se algum vértice do voxel full estiver contido no filho -> filho vira parcial
								// mas se for folha -> filho vira full
								vector<Vector4Df> voxelVertices = Vertices(treeList[1].tree[k]);
								int nodeCount = 0;
								for (int n = 0; n < 8; n++)
									if (IsContained(voxelVertices[n], kids->at(j)) == true) nodeCount++;
								if (nodeCount > 0)
								{
									//std::cout << "nodeCount = " << nodeCount << std::endl;
									if (kids->at(j).level == deep) { kids->at(j).status = 'F'; break; }
									else { kids->at(j).status = 'P'; break; }
								}
								else
								{
									// se algum vértice do filho parcial estiver contido no voxel full -> filho vira parcial
									// mas se for folha -> filho vira full
									voxelVertices = Vertices(kids->at(j));
									nodeCount = 0;
									for (int n = 0; n < 8; n++)
										if (IsContained(voxelVertices[n], treeList[1].tree[k]) == true) nodeCount++;
									if (nodeCount > 0)
									{
										//std::cout << "nodeCount2 = " << nodeCount << std::endl;
										if (kids->at(j).level == deep) { kids->at(j).status = 'F'; break; }
										else { kids->at(j).status = 'P'; break; }
									}
								}
							}
						}
					}
				}
			}
			tree2.tree[i].childrens = kids;
			// Add filhos
			tree2.tree.insert(tree2.tree.end(), kids->begin(), kids->end());
		}
	}

	//Initialize the INTERSECT TREE
	for (int i = 0; i < interTree.tree.size(); i++)
	{
		if (interTree.tree[i].level < deep && interTree.tree[i].status == 'P')
		{
			vector<box> *kids = new vector<box>(8);
			kids = Divide(interTree.tree[i]);

			// State Check
			for (int j = 0; j < 8; j++)
			{
				if (kids->at(j).level == deep) kids->at(j).status = 'E';
				else kids->at(j).status = 'P';
			}
			interTree.tree[i].childrens = kids;
			// Add filhos
			interTree.tree.insert(interTree.tree.end(), kids->begin(), kids->end());
		}
	}

	//INTERSECT
	for (int i = 1; i < interTree.tree.size(); i++)
	{
		//default
		box A, B;
		A.status = 'E'; B.status = 'E';
		//box A = subTree.tree[i], B = subTree.tree[i];
		//se achar voxel igual em 1, pega!
		for (int j = 1; j < tree1.tree.size(); j++)
			if (tree1.tree[j].level == interTree.tree[i].level && tree1.tree[j].point == interTree.tree[i].point)
				A = tree1.tree[j];
		////se achar voxel igual em 2, pega!
		for (int j = 1; j < tree2.tree.size(); j++)
			if (tree2.tree[j].level == interTree.tree[i].level && tree2.tree[j].point == interTree.tree[i].point)
				B = tree2.tree[j];

		//test
		if (A.status == 'F' && B.status == 'F') interTree.tree[i].status = 'F';
		if (A.status == 'E' || B.status == 'E') interTree.tree[i].status = 'E';
	}

	//CLOSE
	treeList.clear();
	treeList.push_back(interTree);
	/*treeList.push_back(tree1);
	treeList.push_back(tree2);*/
}

/* ===================== * MASS PROPERTY * ===================== */

string Octree::Volume()
{
	//Initialize
	string report;
	clock_t begin = clock();

	report = "---------------   OCTREES   --------------";

	for (int i = 0; i < treeList.size(); i++)
	{
		float volume = 0.0f;
		for (int j = 1; j < treeList[i].tree.size(); j++)
			if (treeList[i].tree[j].status == 'F')
			{
				/*cálculo do volume*/
				volume += powf(treeList[i].tree[j].size, 3.0f);
			}

		report += "\n";
		report += "\n "; report += treeList[i].name;
		report += "\n";
		report += "\nVolume:\t"; report += to_string(volume); report += " unid³";
	}

	clock_t end = clock();
	float elapsed_secs = float(end - begin) / CLOCKS_PER_SEC;
	std::cout << "Elapsed secs = " << elapsed_secs << std::endl;

	return report;
}

string Octree::Area()
{
	//Initialize
	string report;
	clock_t begin = clock();

	report = "---------------   OCTREES   --------------";

	for (int i = 0; i < treeList.size(); i++)
	{
		float area = 0.0f;
		for (int j = 1; j < treeList[i].tree.size(); j++)
			if (treeList[i].tree[j].status == 'F')
			{
				/*cálculo da área*/
				Vector4Df neighbor[6];
				neighbor[0] = { treeList[i].tree[j].point.x - treeList[i].tree[j].size, treeList[i].tree[j].point.y, treeList[i].tree[j].point.z, treeList[i].tree[j].point.w };
				neighbor[1] = { treeList[i].tree[j].point.x + treeList[i].tree[j].size, treeList[i].tree[j].point.y, treeList[i].tree[j].point.z, treeList[i].tree[j].point.w };
				neighbor[2] = { treeList[i].tree[j].point.x, treeList[i].tree[j].point.y - treeList[i].tree[j].size, treeList[i].tree[j].point.z, treeList[i].tree[j].point.w };
				neighbor[3] = { treeList[i].tree[j].point.x, treeList[i].tree[j].point.y + treeList[i].tree[j].size, treeList[i].tree[j].point.z, treeList[i].tree[j].point.w };
				neighbor[4] = { treeList[i].tree[j].point.x, treeList[i].tree[j].point.y, treeList[i].tree[j].point.z - treeList[i].tree[j].size, treeList[i].tree[j].point.w };
				neighbor[5] = { treeList[i].tree[j].point.x, treeList[i].tree[j].point.y, treeList[i].tree[j].point.z + treeList[i].tree[j].size, treeList[i].tree[j].point.w };
				for (int k = 0; k < 6; k++)
				{
					//se vizinho fora da Bounding Box
					Vector4Df W = neighbor[k] - treeList[i].tree[0].point;
					if (W.x >= treeList[i].tree[0].size || W.x < 0.0f || W.y >= treeList[i].tree[0].size || W.y < 0.0f || W.z >= treeList[i].tree[0].size || W.z < 0.0f)
					{
						area += powf(treeList[i].tree[j].size, 2.0f);
						continue;
					}
					for (int m = 1; m < treeList[i].tree.size(); m++)
						if (treeList[i].tree[m].point == neighbor[k] && treeList[i].tree[m].level == treeList[i].tree[j].level)
						{
							if (treeList[i].tree[m].status == 'F') continue;
							if (treeList[i].tree[m].status == 'E') area += powf(treeList[i].tree[j].size, 2.0f);
						}
				}
			}

		report += "\n";
		report += "\n "; report += treeList[i].name;
		report += "\n";
		report += "\nArea:\t"; report += to_string(area); report += " unid²";
	}

	clock_t end = clock();
	float elapsed_secs = float(end - begin) / CLOCKS_PER_SEC;
	std::cout << "Elapsed secs = " << elapsed_secs << std::endl;

	return report;
}

/* ===================== * READ TREE FILE * ===================== */

bool Octree::ReadTreeFile()
{
	string inName = "tree.txt";
	ifstream inFile(inName);
	if (!inFile.is_open())
	{
		cerr << "\n\t!!! FILE COULD NOT BE OPENED !!!\n" << endl;
		return false;
	}

	//Initialize
	treeList.clear();
	//Read a file line by line
	string line;
	ocTree currentTree;
	currentTree.name = "TREE";
	bool object = false;

	while (!inFile.eof())
	{
		getline(inFile, line);

		/*-------------------* OCTREE *-------------------*/

		box BB;

		if (line.substr(0, 1) == "1")
		{
			int status;

			//Bounding Box
			line = line.substr(1);
			istringstream s(line);
			s >> BB.point.x;
			s >> BB.point.y;
			s >> BB.point.z;
			s >> BB.size;
			s >> status;
			if (status == 2) BB.status = 'P';
			else if (status == 1) BB.status = 'F';
			else BB.status = 'E';
			BB.level = 0;
			BB.parent = NULL;
			currentTree.tree.push_back(BB);
		}
	}
	//END OF FILE

	treeList.push_back(currentTree);

	std::cout << "\n\tFile [" << inName << "] has been opened successfully!" << std::endl;

	return true;
}

/* ===================== * WRITE TREE FILE * ===================== */

void Octree::WriteTreeFile()
{
	string outName = "tree.txt";
	ofstream outFile(outName);
	if (!outFile.is_open())
	{
		cerr << "\n\t!!! FILE COULD NOT BE OPENED !!!\n" << std::endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	/*------------------* OBJECTS *-------------------*/

	/*for (*/int i = 0;/* i < treeList.size(); i++)*/
	{
		vector<box> currentTree = treeList[i].tree;
		//type
		outFile << "1";
		//bounding-box point
		outFile << " " << currentTree[0].point.x << " " << currentTree[0].point.y << " " << currentTree[0].point.z + currentTree[0].size;
		//bounding-box size
		outFile << " " << currentTree[0].size;
		//root (bounding-box)
		outFile << " 2";
		currentTree.erase(currentTree.begin());

		int j = 0;
		int leafCount = 0;
		int lastLevel = 0;
		vector<lastNode> lastList;
		while (!currentTree.empty())
		{
			/*std::cout << "\n=================== LOOP ==================\n" << std::endl;
			std::cout << "current tree size = " << currentTree.size() << std::endl;
			std::cout << "j = " << j << std::endl;*/
			/*system("pause");*/

			if (leafCount == 8 && lastList.size() > 0)
			{
				j = lastList[lastList.size() - 1].position;
				leafCount = lastList[lastList.size() - 1].leaf;
				lastList.pop_back();
			}

			if (currentTree[j].status == 'F')
			{
				outFile << " 1";
				leafCount++;

				currentTree.erase(currentTree.begin() + j);
			}
			else if (currentTree[j].status == 'E')
			{
				outFile << " 0";
				leafCount++;

				currentTree.erase(currentTree.begin() + j);
			}
			else if (currentTree[j].status == 'P')
			{
				outFile << " 2";
				leafCount++;
				lastLevel = currentTree[j].level;

				currentTree.erase(currentTree.begin() + j);

				//armazena info do último nó
				if (leafCount < 8)
				{
					lastNode last;
					last.position = j;
					last.leaf = leafCount;
					lastList.push_back(last);
				}

				leafCount = 0;
				//procura onde começa a próx profundidade
				/* ---------------- USAR PONTEIRO PARA ACELERAR ---------------- */
				for (int k = 0; k < currentTree.size(); k++)
					if (currentTree[k].level == lastLevel + 1) { j = k; break; }
			}
		}
	}

	/*--------------------* END *--------------------*/

	outFile.close();

	std::cout << "\nFile [" << outName << "] has been created successfully!" << std::endl;
}