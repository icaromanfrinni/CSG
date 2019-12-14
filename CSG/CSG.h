#pragma once
#ifndef CSG_H
#define CSG_H

#include <iostream>
#include <vector>
#include "Linear_Algebra.h"
#include "Object.h"
#include "Box.h"

//------------------------------------------------------------------------------
// TYPES
//------------------------------------------------------------------------------

struct Edge
{
	Vector4Df start, end, direction;
	int status; // 1=IN, 2=ON, 3=OUT
	Object obj;

	Edge()
	{
	}

	Edge(const Vector4Df &_start, const Vector4Df &_end, const int &_status) : start(_start), end(_end), status(_status)
	{
		direction = (end - start).to_unitary();
		//obj = NULL;
	}

	void SetDirection()
	{
		direction = (end - start).to_unitary();
	}
};

struct setMembership
{
	std::vector<Edge> edges;

	setMembership()
	{
	}

	setMembership(const Edge &_edge)
	{
		edges.push_back(_edge);
	}

	void Union(const Edge &edge_B)
	{
		if (edge_B.status == 3) return;

		float t1_B = (edge_B.start - edges[0].start).length();
		float t2_B = (edge_B.end - edges[0].start).length();

		for (int i = 0; i < edges.size(); i++)
		{
			float t1_A = (edges[i].start - edges[0].start).length();
			float t2_A = (edges[i].end - edges[0].start).length();

			if (t1_B > t1_A && t2_B < t2_A) //novo Edge totalmente dentro do edge existente
			{
				if (edges[i].status == 3)
				{
					std::vector<Edge> addEdgeList;
					addEdgeList.push_back(edge_B);
					Edge finalSegment = Edge(edge_B.end, edges[i].end, edges[i].status);
					finalSegment.obj = edges[i].obj;
					addEdgeList.push_back(finalSegment);
					edges[i].end = edge_B.start;
					edges.insert(edges.begin() + i + 1, addEdgeList.begin(), addEdgeList.end());
					i++;
					i++;
				}
			}
			else if (t1_B < t1_A && t2_B > t2_A) //novo Edge totalmente fora do edge existente
			{
				//if (edges[i].status == 3)
				//{
					edges[i].status = edge_B.status;
					edges[i].obj = edge_B.obj;
				//}
			}
			else if (t1_B > t1_A && t1_B < t2_A && t2_B > t2_A) //apenas 'start' dentro
			{
				//if (edge_B.start == edges[i].end) continue;
				//if (edges[i].status == 3)
				//{
					std::vector<Edge> addEdgeList;
					Edge newSegment = Edge(edge_B.start, edges[i].end, edge_B.status);
					newSegment.obj = edge_B.obj;
					addEdgeList.push_back(newSegment);
					edges[i].end = edge_B.start;
					edges.insert(edges.begin() + i + 1, addEdgeList.begin(), addEdgeList.end());
					i++;
				//}
			}
			else if (t1_B < t1_A && t2_B > t1_A && t2_B < t2_A) //apenas 'end' dentro
			{
				//if (edge_B.end == edges[i].start) continue;
				//if (edges[i].status == 3)
				//{
					std::vector<Edge> addEdgeList;
					Edge newSegment = Edge(edge_B.end, edges[i].end, edges[i].status);
					newSegment.obj = edges[i].obj;
					addEdgeList.push_back(newSegment);
					edges[i].end = edge_B.end;
					edges[i].status = edge_B.status;
					edges[i].obj = edge_B.obj;
					edges.insert(edges.begin() + i + 1, addEdgeList.begin(), addEdgeList.end());
					i++;
				//}
			}
		}
		//regularize
		//Regularize();
	}

	void Subtract(const Edge &edge_B)
	{
		if (edge_B.status == 3) return;

		float t1_B = (edge_B.start - edges[0].start).length();
		float t2_B = (edge_B.end - edges[0].start).length();

		for (int i = 0; i < edges.size(); i++)
		{
			float t1_A = (edges[i].start - edges[0].start).length();
			float t2_A = (edges[i].end - edges[0].start).length();

			if (edges[i].status == 1)
			{
				if (t1_B > t1_A && t2_B < t2_A) //novo edge totalmente dentro
				{
					std::vector<Edge> addEdgeList;
					addEdgeList.push_back(Edge(edge_B.start, edge_B.end, 3));
					Edge newSegment = Edge(edge_B.end, edges[i].end, edges[i].status);
					newSegment.obj = edges[i].obj;
					addEdgeList.push_back(newSegment);
					edges[i].end = edge_B.start;
					edges.insert(edges.begin() + i + 1, addEdgeList.begin(), addEdgeList.end());
					i++;
					i++;
				}
				else if (t1_B < t1_A && t2_B > t2_A) //novo edge totalmente fora
				{
					edges[i] = Edge(edges[i].start, edges[i].end, 3);
				}
				else if (edges[i].start == edge_B.start && edges[i].end == edge_B.end) //novo edge IGUAL
				{
					edges[i] = Edge(edges[i].start, edges[i].end, 3);
				}
				else if (t1_A > t1_B && t1_A < t2_B && t2_A > t2_B) //apenas 'end' dentro
				{
					std::vector<Edge> addEdgeList;
					Edge newSegment = Edge(edge_B.end, edges[i].end, edges[i].status);
					newSegment.obj = edge_B.obj;
					addEdgeList.push_back(newSegment);
					edges[i] = Edge(edges[i].start, edge_B.end, 3);
					edges.insert(edges.begin() + i + 1, addEdgeList[0]);
					i++;
				}
				else if (t1_A < t1_B && t2_A > t1_B && t2_A < t2_B) //apenas 'start' dentro
				{
					std::vector<Edge> addEdgeList;
					Edge newSegment = Edge(edge_B.start, edges[i].end, 3);
					addEdgeList.push_back(newSegment);
					edges[i].end = edge_B.start;
					edges.insert(edges.begin() + i + 1, addEdgeList[0]);
					i++;
				}
			}
		}
		//regularize
		//Regularize();
	}

	void Intersect(const Edge &edge_B)
	{
		float t1_B = (edge_B.start - edges[0].start).length();
		float t2_B = (edge_B.end - edges[0].start).length();

		for (int i = 0; i < edges.size(); i++)
		{
			float t1_A = (edges[i].start - edges[0].start).length();
			float t2_A = (edges[i].end - edges[0].start).length();

			if (edges[i].status == 1)
			{
				if (t1_B > t1_A && t2_B < t2_A) //novo edge totalmente dentro
				{
					std::vector<Edge> addEdgeList;
					addEdgeList.push_back(edge_B);
					addEdgeList.push_back(Edge(edge_B.end, edges[i].end, 3));
					edges[i] = Edge(edges[i].start, edge_B.start, 3);
					edges.insert(edges.begin() + i + 1, addEdgeList.begin(), addEdgeList.end());
					i++;
					i++;
				}
				else if (t1_B < t1_A && t2_B > t2_A) //novo edge totalmente fora
				{
					if (edge_B.status == 3) edges[i] = Edge(edges[i].start, edges[i].end, 3);
				}
				else if (edges[i].start == edge_B.start && edges[i].end == edge_B.end) //novo edge IGUAL
				{
					edges[i].obj = edge_B.obj;
				}
				else if (t1_A > t1_B && t1_A < t2_B && t2_A > t2_B) //apenas 'end' dentro
				{
					std::vector<Edge> addEdgeList;
					addEdgeList.push_back(Edge(edge_B.end, edges[i].end, 3));
					edges[i].end = edge_B.end;
					//edges[i].obj = edge_B.obj;
					edges.insert(edges.begin() + i + 1, addEdgeList[0]);
					i++;
				}
				else if (t1_A < t1_B && t2_A > t1_B && t2_A < t2_B) //apenas 'start' dentro
				{
					std::vector<Edge> addEdgeList;
					Edge newSegment = Edge(edge_B.start, edges[i].end, 1);
					newSegment.obj = edge_B.obj;
					addEdgeList.push_back(newSegment);
					edges[i] = Edge(edges[i].start, edge_B.start, 3);
					edges.insert(edges.begin() + i + 1, addEdgeList[0]);
					i++;
				}
				else edges[i] = Edge(edges[i].start, edges[i].end, 3);
			}
		}
		//regularize
		//Regularize();
	}

	//TODO
	void Regularize()
	{
		Edge lastEdge = edges[0];
		for (int i = 1; i < edges.size(); i++)
		{
			if (edges[i].status == lastEdge.status)
			{
				edges[i].start = lastEdge.start;
				edges[i].obj = lastEdge.obj;
				edges.erase(edges.begin() + i - 1);
				i--;
			}

			lastEdge = edges[i];
		}
	}
};

//------------------------------------------------------------------------------
// CLASSES
//------------------------------------------------------------------------------

class CSG
{
public:
	//DEFAULT CONSTRUCTOR
	CSG();
	//DESTRUCTOR
	~CSG();

	//BOOLEAN OPERATIONS
	void Union(const std::vector<Object> &objList);
	void Subtract(const std::vector<Object> &objList);
	void Intersect(const std::vector<Object> &objList);

	//MASS PROPERTIES
	box BoundingBox(const std::vector<Object>& objList) const;
	float Volume(const std::vector<Object>& objList, const float &resolution) const;

	std::vector<Edge> edgeList;
};

#endif // CSG_H