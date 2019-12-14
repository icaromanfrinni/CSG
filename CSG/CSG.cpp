#include "CSG.h"

#include <iostream>
#include "Ray.h"

#define DEBUG 1

//DEFAULT CONSTRUCTOR
CSG::CSG()
{
	edgeList.clear();
}

//DESTRUCTOR
CSG::~CSG()
{
}

/* --------------------- MASS PROPERTIES --------------------- */

box CSG::BoundingBox(const std::vector<Object>& objList) const
{
	box BB = objList[0].getBoundingBox();
	Vector4Df pMin = BB.point;
	Vector4Df pMax = BB.point;

	for (int i = 0; i < objList.size(); i++)
	{
		//Min
		if (objList[i].getBoundingBox().point.x < pMin.x) pMin.x = objList[i].getBoundingBox().point.x;
		if (objList[i].getBoundingBox().point.y < pMin.y) pMin.y = objList[i].getBoundingBox().point.y;
		if (objList[i].getBoundingBox().point.z < pMin.z) pMin.z = objList[i].getBoundingBox().point.z;
		//Max
		if (objList[i].getBoundingBox().point.x + objList[i].getBoundingBox().size > pMax.x) pMax.x = objList[i].getBoundingBox().point.x + objList[i].getBoundingBox().size;
		if (objList[i].getBoundingBox().point.y + objList[i].getBoundingBox().size > pMax.y) pMax.y = objList[i].getBoundingBox().point.y + objList[i].getBoundingBox().size;
		if (objList[i].getBoundingBox().point.z + objList[i].getBoundingBox().size > pMax.z) pMax.z = objList[i].getBoundingBox().point.z + objList[i].getBoundingBox().size;
	}

	//Max difference between extreme points = size of bounding box
	BB.size = pMax.x - pMin.x;
	if (pMax.y - pMin.y > BB.size)
		BB.size = pMax.y - pMin.y;
	if (pMax.z - pMin.z > BB.size)
		BB.size = pMax.z - pMin.z;

	BB.point = pMin - Vector4Df{ 1.0f, 1.0f, 1.0f, 0.0f }; //box point reference
	BB.size += 2.0f;
	return BB;
}

float CSG::Volume(const std::vector<Object>& objList, const float &resolution) const
{
	float v = 0.0f;
	box Global_BB = BoundingBox(objList);
	float ray_width = Global_BB.size / resolution;

	for (int m = 0; m < resolution; m++)
		for (int n = 0; n < resolution; n++)
		{
			Ray ray;
			ray.origin = Global_BB.point + Vector4Df{m * ray_width, n * ray_width, 0.0f, 0.0f};
			ray.direction = { 0.0f, 0.0f, 1.0f, 0.0f };

			setMembership setRay(Edge(ray.origin, ray.direction * Global_BB.size, 3));
			for (int i = 0; i < objList.size(); i++)
			{
				// collision list
				RayCollisionList dist_List = objList[i].CollideAll(ray);

				Edge currentMembership;
				// get the current object
				if (dist_List.collisions.size() == 2)
				{
					currentMembership = Edge(dist_List.collisions[0].point, dist_List.collisions[1].point, 1);
					currentMembership.obj = objList[i];
				}
				else
				{
					currentMembership = Edge(ray.origin, ray.direction * Global_BB.size, 3);
				}

				/* ÁRVORE CSG */
				if (objList[i].op == 1)
					setRay.Union(currentMembership);
				else if (objList[i].op == 2)
					setRay.Subtract(currentMembership);
				else if (objList[i].op == 3)
					setRay.Intersect(currentMembership);
			}

			for (int i = 0; i < setRay.edges.size(); i++)
				if (setRay.edges[i].status == 1)
					v += ray_width * ray_width * (setRay.edges[i].end - setRay.edges[i].start).length();
		}

	return v;
}

/* --------------------- BOOLEAN OPERATIONS --------------------- */

void CSG::Union(const std::vector<Object> &objList)
{
	if (edgeList.empty() == true) return;

	Ray e;
	e.origin = edgeList[0].start;
	e.direction = edgeList[0].direction;

	// TODAS AS COLISÕES

	RayCollisionList t_List;
	if (objList.empty() == true) return;
	else t_List = objList.back().CollideAll(e);

	if (t_List.collisions.size() == 2)
	{
		Vector4Df p_start = e.origin + (e.direction * t_List.collisions[0].t);
		Vector4Df p_end = e.origin + (e.direction * t_List.collisions[1].t);

		for (int i = 0; i < edgeList.size(); i++)
		{
			std::cout << "i = " << i << std::endl;

			float t_start = (edgeList[i].start - e.origin).length();
			float t_end = (edgeList[i].end - e.origin).length();

			if (t_start > t_List.collisions[0].t && t_end < t_List.collisions[1].t) //totalmente fora
			{
				edgeList[i].status = 1;
			}
			else if (t_start < t_List.collisions[0].t && t_end > t_List.collisions[1].t) //totalmente dentro
			{
				std::vector<Edge> newSegments;
				newSegments.push_back(Edge(t_List.collisions[0].point, t_List.collisions[1].point, 1));
				newSegments.push_back(Edge(t_List.collisions[1].point, edgeList[i].end, edgeList[i].status));
				edgeList[i].end = t_List.collisions[0].point;
				edgeList.insert(edgeList.begin() + i + 1, newSegments.begin(), newSegments.end()); //inserir novos segmentos na posição após o segmento corrente (i + 1)
				i++;
				i++;
			}
			else if (t_start < t_List.collisions[0].t && t_end > t_List.collisions[0].t && t_end < t_List.collisions[1].t) //apenas 'end' dentro
			{
				//verifica se o final do segmento corrente é igual à primeira colisão
				if (edgeList[i].end == t_List.collisions[0].point) continue;

				std::vector<Edge> newSegments;
				newSegments.push_back(Edge(t_List.collisions[0].point, edgeList[i].end, 1));
				edgeList[i].end = t_List.collisions[0].point;
				edgeList.insert(edgeList.begin() + i + 1, newSegments[0]);
				i++;
			}
			else if (t_start > t_List.collisions[0].t && t_start < t_List.collisions[1].t && t_end > t_List.collisions[1].t) //apenas 'start' dentro
			{
				//verifica se o início do segmento corrente é igual à segunda colisão
				if (edgeList[i].start == t_List.collisions[1].point) continue;

				std::vector<Edge> newSegments;
				newSegments.push_back(Edge(t_List.collisions[1].point, edgeList[i].end, edgeList[i].status));
				edgeList[i].end = t_List.collisions[1].point;
				edgeList[i].status = 1;
				edgeList.insert(edgeList.begin() + i + 1, newSegments[0]);
				i++;
			}
		}
	}
}

void CSG::Subtract(const std::vector<Object> &objList)
{
	if (edgeList.empty() == true) return;

	Ray e;
	e.origin = edgeList[0].start;
	e.direction = edgeList[0].direction;

	RayCollisionList t_List;
	if (objList.empty() == true) return;
	else t_List = objList.back().CollideAll(e);

	if (t_List.collisions.size() == 2)
	{
		Vector4Df p_start = e.origin + (e.direction * t_List.collisions[0].t);
		Vector4Df p_end = e.origin + (e.direction * t_List.collisions[1].t);

		for (int i = 0; i < edgeList.size(); i++)
		{
			float t_start = (edgeList[i].start - e.origin).length();
			float t_end = (edgeList[i].end - e.origin).length();

			if (edgeList[i].status == 1)
			{
				if (t_start > t_List.collisions[0].t && t_start < t_List.collisions[1].t && t_end > t_List.collisions[1].t) //apenas 'end' dentro
				{
					std::vector<Edge> addEdgeList;
					Edge newSegment = Edge(t_List.collisions[1].point, edgeList[i].end, edgeList[i].status);
					newSegment.obj = edgeList[i].obj;
					addEdgeList.push_back(newSegment);
					edgeList[i] = Edge(edgeList[i].start, t_List.collisions[1].point, 3);
					edgeList.insert(edgeList.begin() + i + 1, addEdgeList[0]);
					i++;
				}
				else if (t_start < t_List.collisions[0].t && t_end > t_List.collisions[0].t && t_end < t_List.collisions[1].t) //apenas 'start' dentro
				{
					std::vector<Edge> addEdgeList;
					Edge newSegment = Edge(t_List.collisions[0].point, edgeList[i].end, 3);
					addEdgeList.push_back(newSegment);
					edgeList[i].end = t_List.collisions[0].point;
					edgeList.insert(edgeList.begin() + i + 1, addEdgeList[0]);
					i++;
				}
				else if (t_start < t_List.collisions[0].t && t_end > t_List.collisions[1].t) //novo edge totalmente dentro
				{
					std::vector<Edge> addEdgeList;
					addEdgeList.push_back(Edge(t_List.collisions[0].point, t_List.collisions[1].point, 3));
					Edge newSegment = Edge(t_List.collisions[1].point, edgeList[i].end, edgeList[i].status);
					newSegment.obj = edgeList[i].obj;
					addEdgeList.push_back(newSegment);
					edgeList[i].end = t_List.collisions[0].point;
					edgeList.insert(edgeList.begin() + i + 1, addEdgeList.begin(), addEdgeList.end());
					i++;
					i++;
				}
				else if (t_start > t_List.collisions[0].t && t_end < t_List.collisions[1].t) //novo edge totalmente fora
				{
					edgeList[i] = Edge(edgeList[i].start, edgeList[i].end, 3);
				}
			}


			
		}
	}
}

void CSG::Intersect(const std::vector<Object> &objList)
{
	if (edgeList.empty() == true) return;

	Ray e;
	e.origin = edgeList[0].start;
	e.direction = edgeList[0].direction;

	RayCollisionList t_List;
	if (objList.empty() == true) return;
	else t_List = objList.back().CollideAll(e);

	if (t_List.collisions.size() == 2)
	{
		Vector4Df p_start = e.origin + (e.direction * t_List.collisions[0].t);
		Vector4Df p_end = e.origin + (e.direction * t_List.collisions[1].t);

		for (int i = 0; i < edgeList.size(); i++)
		{
			float t_start = (edgeList[i].start - e.origin).length();
			float t_end = (edgeList[i].end - e.origin).length();

			if (edgeList[i].status == 1)
			{
				if (t_List.collisions[0].t > t_start && t_List.collisions[1].t < t_end) //novo edge totalmente dentro
				{
					std::vector<Edge> addEdgeList;
					addEdgeList.push_back(Edge(t_List.collisions[0].point, t_List.collisions[1].point, 1));
					addEdgeList.push_back(Edge(t_List.collisions[1].point, edgeList[i].end, 3));
					edgeList[i] = Edge(edgeList[i].start, t_List.collisions[0].point, 3);
					edgeList.insert(edgeList.begin() + i + 1, addEdgeList.begin(), addEdgeList.end());
					i++;
					i++;
				}
				else if (t_List.collisions[0].t < t_start && t_List.collisions[1].t > t_end) //novo edge totalmente fora
				{
					// não faz nada
					//if (edge_B.status == 3) edgeList[i] = Edge(edgeList[i].start, edgeList[i].end, 3);
				}
				else if (edgeList[i].start == t_List.collisions[0].point && edgeList[i].end == t_List.collisions[1].point) //novo edge IGUAL
				{
					// não faz nada
				}
				else if (t_start > t_List.collisions[0].t && t_start < t_List.collisions[1].t && t_end > t_List.collisions[1].t) //apenas 'end' dentro
				{
					std::vector<Edge> addEdgeList;
					addEdgeList.push_back(Edge(t_List.collisions[1].point, edgeList[i].end, 3));
					edgeList[i].end = t_List.collisions[1].point;
					edgeList.insert(edgeList.begin() + i + 1, addEdgeList[0]);
					i++;
				}
				else if (t_start < t_List.collisions[0].t && t_end > t_List.collisions[0].t && t_end < t_List.collisions[1].t) //apenas 'start' dentro
				{
					std::vector<Edge> addEdgeList;
					Edge newSegment = Edge(t_List.collisions[0].point, edgeList[i].end, 1);
					addEdgeList.push_back(newSegment);
					edgeList[i] = Edge(edgeList[i].start, t_List.collisions[0].point, 3);
					edgeList.insert(edgeList.begin() + i + 1, addEdgeList[0]);
					i++;
				}
			}



		}
	}
}