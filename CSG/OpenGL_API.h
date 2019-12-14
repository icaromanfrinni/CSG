#pragma once

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <Windows.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl2.h"

#include "Linear_Algebra.h"
#include "Camera.h"
#include "OpenGL_Render.h"

#include "Object.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Cube.h"
#include "CSG.h"

#include "RayCast/RenderAPI.h"

#define DEBUG 0

namespace OpenGL_API
{
	const int widthRender = 512, heightRender = 512;

	/* CAMERA */
	Camera cam = Camera({ 10.0f, 5.0f, 10.0f, 1.0f },	// viewpoint
	{ 0.0f, 0.0f, 0.0f, 1.0f },		// lookat
	{ 0.0f, 1.0f, 0.0f, 0.0f },		// view up
	float2{ widthRender*1.0f, heightRender*1.0f }, //resolution
	float2{ 1.0f, 1.0f },			//dimensions
	1.0f //near
	);	

	//LIGHT
	GLfloat posicaoLuz[4] = { 10.0f, 10.0f, 10.0f, 1.0f };

	// mouse event handlers
	Vector4Df last = { 0.0f, 0.0f, 0.0f, 0.0f };
	int TheButtonState = 0;
	int TheKeyState = 0;
	float panSpeed = 0.05f;

	// keyboard event handlers
	int index = 0; //to select the current tree
	bool MoveKey = false;
	bool ScaleKey = false;

	//list of materials
	Material *Neutral = new Material(Vector4Df{ 0.1f, 0.1f, 0.1f, 0 }, Vector4Df{ 1.0f, 1.0f, 1.0f, 0 }, Vector4Df{ 1.0f, 1.0f, 1.0f, 0 }, 1000);
	Material *Red = new Material(Vector4Df{ 0.1f, 0.0f, 0.0f, 0 }, Vector4Df{ 1.0f, 0.2f, 0.2f, 0 }, Vector4Df{ 1.0f, 1.0f, 1.0f, 0 }, 10);
	Material *Green = new Material(Vector4Df{ 0.0f, 0.1f, 0.0f, 0 }, Vector4Df{ 0.2f, 1.0f, 0.2f, 0 }, Vector4Df{ 1.0f, 1.0f, 1.0f, 0 }, 10);
	Material *Blue = new Material(Vector4Df{ 0.0f, 0.0f, 0.1f, 0 }, Vector4Df{ 0.2f, 0.2f, 1.0f, 0 }, Vector4Df{ 1.0f, 1.0f, 1.0f, 0 }, 10);
	Material *Yellow = new Material(Vector4Df{ 0.1f, 0.1f, 0.0f, 0 }, Vector4Df{ 1.0f, 1.0f, 0.2f, 0 }, Vector4Df{ 1.0f, 1.0f, 1.0f, 0 }, 10);

	//list of objects
	std::vector<Object> objs;/* = { Object("Esfera", Neutral, new Sphere({ 0.0f, 3.0f, 0.0f, 1.0f }, 2.0f)) };*/
	std::vector<Cube> cubes;
	std::vector<Sphere> spheres;/* = { Sphere({ 0.0f, 3.0f, 0.0f, 1.0f }, 2.0f) };*/
	std::vector<Cylinder> cylinders;
	std::vector<Cone> cones;

	//CSG
	CSG objCSG;

	//Menu
	static bool show_app_property_editor = false;
	static bool show_mass_window = false;

	/* ======================================= MENU FUNCTIONS ======================================= */

	void Render_MENU()
	{
		RenderAPI::cam = cam;
		RenderAPI::cam.newUp();
		RenderAPI::objs = objs;
		RenderAPI::renderEnable = true;
	}

	void New_MENU()
	{
		TheButtonState = 0;
		// keyboard event handlers
		index = 0;
		MoveKey = false;
		ScaleKey = false;
		//list of objects
		RenderAPI::objs.clear();
		objs.clear();
		cubes.clear();
		spheres.clear();
		cylinders.clear();
		cones.clear();
		//CSG
		objCSG.edgeList.clear();
	}

	/* .............................................................................................. */
	/* ========================================== EXAMPLES ========================================== */
	/* .............................................................................................. */

	void TEMA()
	{
		std::cout << "TEMA" << std::endl;

		cam.eye = { 2.5f, 5.0f, 25.0f, 1.0f };
		cam.lookat = { -2.0f, 5.0f, 0.0f, 1.0f };
		cam.up = { 0.0f, 1.0f, 0.0f, 0.0f };

		objs.push_back(Object("Cubo", Neutral, new Cube({ 0.0f, 7.875f, 0.0f, 1.0f }, 8.0f, 0.25f, 30.0f)));
		objs.back().op = 1;
		cubes.push_back(Cube({ 0.0f, 7.875f, 0.0f, 1.0f }, 8.0f, 0.25f, 30.0f)); //center, size
		cubes.back().id = objs.size() - 1;

		
		objs.push_back(Object("Cubo", Neutral, new Cube({ -3.80f, 8.30f, 0.0f, 1.0f }, 0.4f, 0.6f, 30.0f)));
		objs.back().op = 1;
		cubes.push_back(Cube({ -3.80f, 8.30f, 0.0f, 1.0f }, 0.4f, 0.6f, 30.0f)); //center, size
		cubes.back().id = objs.size() - 1;

		
		objs.push_back(Object("Cubo", Neutral, new Cube({ 3.80f, 8.30f, 0.0f, 1.0f }, 0.4f, 0.6f, 30.0f)));
		objs.back().op = 1;
		cubes.push_back(Cube({ 3.80f, 8.30f, 0.0f, 1.0f }, 0.4f, 0.6f, 30.0f)); //center, size
		cubes.back().id = objs.size() - 1;

		
		objs.push_back(Object("Cubo", Neutral, new Cube({ 0.0f, 7.25f, 0.0f, 1.0f }, 4.0f, 1.5f, 30.0f)));
		objs.back().op = 1;
		cubes.push_back(Cube({ 0.0f, 7.25f, 0.0f, 1.0f }, 4.0f, 1.5f, 30.0f)); //center, size
		cubes.back().id = objs.size() - 1;

		
		objs.push_back(Object("Cubo", Neutral, new Cube({ 0.0f, 7.25f, 0.0f, 1.0f }, 3.5f, 1.0f, 31.0f)));
		objs.back().op = 2;
		cubes.push_back(Cube({ 0.0f, 7.25f, 0.0f, 1.0f }, 3.5f, 1.0f, 31.0f)); //center, size
		cubes.back().id = objs.size() - 1;

		
		objs.push_back(Object("Cilindro", Green, new Cylinder(6.5f, 0.75f, { 0.0f, 0.0f, 10.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f })));
		objs.back().op = 1;
		cylinders.push_back(Cylinder(6.5f, 0.75f, { 0.0f, 0.0f, 10.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f }));
		cylinders.back().id = objs.size() - 1;

		objs.push_back(Object("Cilindro", Green, new Cylinder(6.5f, 0.75f, { 0.0f, 0.0f, -10.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f })));
		objs.back().op = 1;
		cylinders.push_back(Cylinder(6.5f, 0.75f, { 0.0f, 0.0f, -10.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f }));
		cylinders.back().id = objs.size() - 1;
		
	}

	void Example_1()
	{
		std::cout << "EXAMPLE_1" << std::endl;

		objCSG.edgeList.push_back(Edge({ -4.0f, 0.0f, 0.0f, 1.0f }, { 4.0f, 2.0f, 0.0f, 1.0f }, 3));

		objs.push_back(Object("Esfera", Red, new Sphere({ 0.0f, 1.0f, 0.0f, 1.0f }, 1.5f)));
		objs.back().op = 1;
		spheres.push_back(Sphere({ 0.0f, 1.0f, 0.0f, 1.0f }, 1.5f)); //center, radius
		spheres.back().id = objs.size() - 1;
		objCSG.Union(objs);

		objs.push_back(Object("Cubo", Neutral, new Cube({ 0.0f, 0.0f, 0.0f, 1.0f }, 3.0f, 3.0f, 3.0f)));
		objs.back().op = 2;
		cubes.push_back(Cube({ 0.0f, 0.0f, 0.0f, 1.0f }, 3.0f, 3.0f, 3.0f)); //center, size
		cubes.back().id = objs.size() - 1;
		objCSG.Subtract(objs);

		objs.push_back(Object("Cilindro", Green, new Cylinder(5.0f, 1.0f, { -2.5f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 0.0f })));
		objs.back().op = 1;
		cylinders.push_back(Cylinder(5.0f, 1.0f, { -2.5f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 0.0f }));
		cylinders.back().id = objs.size() - 1;
		objCSG.Union(objs);

		objs.push_back(Object("Cilindro", Green, new Cylinder(4.0f, 1.0f, { 0.0f, -2.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f })));
		objs.back().op = 1;
		cylinders.push_back(Cylinder(4.0f, 1.0f, { 0.0f, -2.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f }));
		cylinders.back().id = objs.size() - 1;
		objCSG.Union(objs);
	}

	void Example_2()
	{
		std::cout << "EXAMPLE_2" << std::endl;
		cam.eye = { 5.0f, 5.0f, 5.0f, 1.0f };
		cam.lookat = { 0.0f, 0.0f, 0.0f, 1.0f };

		objCSG.edgeList.push_back(Edge({ -5.0f, -5.5f, 0.0f, 1.0f }, { 5.0f, 5.5f, 0.0f, 1.0f }, 3));

		objs.push_back(Object("Cubo", Neutral, new Cube({ 0.0f, 0.0f, 0.0f, 1.0f }, 3.0f, 3.0f, 3.0f)));
		objs.back().op = 1;
		cubes.push_back(Cube({ 0.0f, 0.0f, 0.0f, 1.0f }, 3.0f, 3.0f, 3.0f)); //center, size
		cubes.back().id = objs.size() - 1;
		objCSG.Union(objs);

		objs.push_back(Object("Esfera", Red, new Sphere({ 0.0f, 0.0f, 0.0f, 1.0f }, 2.0f)));
		objs.back().op = 3;
		spheres.push_back(Sphere({ 0.0f, 0.0f, 0.0f, 1.0f }, 2.0f)); //center, radius
		spheres.back().id = objs.size() - 1;
		objCSG.Intersect(objs);

		objs.push_back(Object("Cilindro", Green, new Cylinder(5.0f, 1.0f, { -2.5f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 0.0f })));
		objs.back().op = 2;
		cylinders.push_back(Cylinder(5.0f, 1.0f, { -2.5f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 0.0f }));
		cylinders.back().id = objs.size() - 1;
		objCSG.Subtract(objs);

		objs.push_back(Object("Cilindro", Green, new Cylinder(5.0f, 1.0f, { 0.0f, -2.5f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f })));
		objs.back().op = 2;
		cylinders.push_back(Cylinder(5.0f, 1.0f, { 0.0f, -2.5f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 0.0f }));
		cylinders.back().id = objs.size() - 1;
		objCSG.Subtract(objs);

		objs.push_back(Object("Cilindro", Green, new Cylinder(5.0f, 1.0f, { 0.0f, 0.0f, -2.5f, 1.0f }, { 0.0f, 0.0f, 1.0f, 0.0f })));
		objs.back().op = 2;
		cylinders.push_back(Cylinder(5.0f, 1.0f, { 0.0f, 0.0f, -2.5f, 1.0f }, { 0.0f, 0.0f, 1.0f, 0.0f }));
		cylinders.back().id = objs.size() - 1;
		objCSG.Subtract(objs);
	}

	//TODO
	void Example_3()
	{
		std::cout << "EXAMPLE_3" << std::endl;
	}

	/* ======================================= ImGui MAIN MENU ======================================= */

	// create a property editor.
	static void ShowMassPropertyWindow(bool* p_open)
	{
		if (!ImGui::Begin("Mass_Properties", p_open))
		{
			ImGui::End();
			return;
		}

		ImGui::Text("Volume = %.1f", objCSG.Volume(objs, cam.resolution.x));
		if (ImGui::Button("Ok"))
		{
			show_mass_window = false;
		}

		ImGui::End();
	}

	// create a property editor.
	static void ShowAppPropertyEditor(bool* p_open)
	{
		ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
		if (!ImGui::Begin("Property editor", p_open))
		{
			ImGui::End();
			return;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();

		struct funcs
		{
			static void ShowObjectProperties(const char* prefix, Object &primitive, int uid)
			{
				ImGui::PushID(uid);                     
				ImGui::AlignTextToFramePadding();  
				bool node_open = ImGui::TreeNode("Primitive", "%s_%u", prefix, uid);
				ImGui::NextColumn();
				ImGui::AlignTextToFramePadding();
				ImGui::Text(primitive.getName().c_str());
				ImGui::NextColumn();
				if (node_open)
				{
					if (typeid(*primitive.getGeometry()).name() == typeid(Sphere).name())
					{
						int index = 0;
						for (int i = 0; i < spheres.size(); i++)
							if (spheres[i].id == uid) { index = i; break; }

						Sphere *primitive_obj = (Sphere*)primitive.getGeometry();
						ImGui::AlignTextToFramePadding();
						ImGui::TreeNodeEx("Center", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
						ImGui::NextColumn();
						ImGui::SetNextItemWidth(-1);
						//ImGui::DragFloat3(" ", (float*)&sphere->center, 0.1f);
						ImGui::DragFloat3(" ", (float*)&spheres[index].center, 0.1f);
						primitive_obj->center = spheres[index].center;
						ImGui::NextColumn();
						ImGui::TreeNodeEx("Radius", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
						ImGui::NextColumn();
						ImGui::SetNextItemWidth(-1);
						ImGui::DragFloat("  ", (float*)&spheres[index].radius, 0.1f);
						primitive_obj->radius = spheres[index].radius;
						ImGui::NextColumn();
						ImGui::TreeNodeEx("Boolean operation", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
						ImGui::NextColumn();
						ImGui::RadioButton("Uni", &primitive.op, 1); ImGui::SameLine();
						ImGui::RadioButton("Sub", &primitive.op, 2); ImGui::SameLine();
						ImGui::RadioButton("Int", &primitive.op, 3);
						ImGui::NextColumn();
					}

					if (typeid(*primitive.getGeometry()).name() == typeid(Cone).name())
					{
						int index = 0;
						for (int i = 0; i < cones.size(); i++)
							if (cones[i].id == uid) { index = i; break; }

						Cone* primitive_obj = (Cone*)primitive.getGeometry();
						ImGui::AlignTextToFramePadding();
						ImGui::TreeNodeEx("Base", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
						ImGui::NextColumn();
						ImGui::SetNextItemWidth(-1);
						ImGui::DragFloat3(" ", (float*)&cones[index].base_center, 0.1f);
						primitive_obj->base_center = cones[index].base_center;
						ImGui::NextColumn();
						ImGui::TreeNodeEx("Radius", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
						ImGui::NextColumn();
						ImGui::SetNextItemWidth(-1);
						ImGui::DragFloat("  ", (float*)&cones[index].radius, 0.1f);
						primitive_obj->radius = cones[index].radius;
						ImGui::NextColumn();
						ImGui::TreeNodeEx("Height", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
						ImGui::NextColumn();
						ImGui::SetNextItemWidth(-1);
						ImGui::DragFloat("   ", (float*)&cones[index].height, 0.1f);
						primitive_obj->height = cones[index].height;
						ImGui::NextColumn();
						ImGui::TreeNodeEx("Direction", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
						ImGui::NextColumn();
						ImGui::SetNextItemWidth(-1);
						ImGui::DragFloat3("    ", (float*)&cones[index].direction, 0.1f, -1.0f, 1.0f);
						primitive_obj->direction = cones[index].direction.to_unitary();
						//new top vertex
						primitive_obj->SetTopVertex();
						ImGui::NextColumn();
						ImGui::TreeNodeEx("Boolean operation", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
						ImGui::NextColumn();
						ImGui::RadioButton("Uni", &primitive.op, 1); ImGui::SameLine();
						ImGui::RadioButton("Sub", &primitive.op, 2); ImGui::SameLine();
						ImGui::RadioButton("Int", &primitive.op, 3);
						ImGui::NextColumn();
					}

					if (typeid(*primitive.getGeometry()).name() == typeid(Cylinder).name())
					{
						int index = 0;
						for (int i = 0; i < cylinders.size(); i++)
							if (cylinders[i].id == uid) { index = i; break; }

						Cylinder* primitive_obj = (Cylinder*)primitive.getGeometry();
						ImGui::AlignTextToFramePadding();
						ImGui::TreeNodeEx("Base", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
						ImGui::NextColumn();
						ImGui::SetNextItemWidth(-1);
						ImGui::DragFloat3(" ", (float*)&cylinders[index].base_center, 0.1f);
						primitive_obj->base_center = cylinders[index].base_center;
						ImGui::NextColumn();
						ImGui::TreeNodeEx("Radius", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
						ImGui::NextColumn();
						ImGui::SetNextItemWidth(-1);
						ImGui::DragFloat("  ", (float*)&cylinders[index].radius, 0.1f);
						primitive_obj->radius = cylinders[index].radius;
						ImGui::NextColumn();
						ImGui::TreeNodeEx("Height", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
						ImGui::NextColumn();
						ImGui::SetNextItemWidth(-1);
						ImGui::DragFloat("   ", (float*)&cylinders[index].height, 0.1f);
						primitive_obj->height = cylinders[index].height;
						ImGui::NextColumn();
						ImGui::TreeNodeEx("Direction", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
						ImGui::NextColumn();
						ImGui::SetNextItemWidth(-1);
						ImGui::DragFloat3("    ", (float*)&cylinders[index].direction, 0.1f, -1.0f, 1.0f);
						primitive_obj->direction = cylinders[index].direction.to_unitary();
						ImGui::NextColumn();
						ImGui::TreeNodeEx("Boolean operation", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
						ImGui::NextColumn();
						ImGui::RadioButton("Uni", &primitive.op, 1); ImGui::SameLine();
						ImGui::RadioButton("Sub", &primitive.op, 2); ImGui::SameLine();
						ImGui::RadioButton("Int", &primitive.op, 3);
						ImGui::NextColumn();
					}

					if (typeid(*primitive.getGeometry()).name() == typeid(Cube).name())
					{
						int index = 0;
						for (int i = 0; i < cubes.size(); i++)
							if (cubes[i].id == uid) { index = i; break; }

						Cube* primitive_obj = (Cube*)primitive.getGeometry();
						ImGui::AlignTextToFramePadding();
						ImGui::TreeNodeEx("Center", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
						ImGui::NextColumn();
						ImGui::SetNextItemWidth(-1);
						ImGui::DragFloat3(" ", (float*)&cubes[index].center, 0.1f);
						primitive_obj->center = cubes[index].center;
						ImGui::NextColumn();
						ImGui::TreeNodeEx("Size_X", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
						ImGui::NextColumn();
						ImGui::SetNextItemWidth(-1);
						ImGui::DragFloat("  ", (float*)&cubes[index].sizeX, 0.1f);
						primitive_obj->sizeX = cubes[index].sizeX;
						ImGui::NextColumn();
						ImGui::TreeNodeEx("Size_Y", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
						ImGui::NextColumn();
						ImGui::SetNextItemWidth(-1);
						ImGui::DragFloat("   ", (float*)&cubes[index].sizeY, 0.1f);
						primitive_obj->sizeY = cubes[index].sizeY;
						ImGui::NextColumn();
						ImGui::TreeNodeEx("Size_Z", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
						ImGui::NextColumn();
						ImGui::SetNextItemWidth(-1);
						ImGui::DragFloat("    ", (float*)&cubes[index].sizeZ, 0.1f);
						primitive_obj->sizeZ = cubes[index].sizeZ;
						//new faces
						primitive_obj->SetVertexFace();
						ImGui::NextColumn();
						ImGui::TreeNodeEx("Boolean operation", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
						ImGui::NextColumn();
						ImGui::RadioButton("Uni", &primitive.op, 1); ImGui::SameLine();
						ImGui::RadioButton("Sub", &primitive.op, 2); ImGui::SameLine();
						ImGui::RadioButton("Int", &primitive.op, 3);
						ImGui::NextColumn();
					}

					//static float dummy_members[8] = { 0.0f,0.0f,1.0f,3.1416f,100.0f,999.0f };
					//for (int i = 0; i < 8; i++)
					//{
					//	ImGui::PushID(i); // Use field index as identifier.
					//	if (i < 2)
					//	{
					//		//ShowDummyObject("Child", 424242);
					//	}
					//	else
					//	{
					//		// Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
					//		ImGui::AlignTextToFramePadding();
					//		ImGui::TreeNodeEx("Field", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet, "Field_%d", i);
					//		ImGui::NextColumn();
					//		ImGui::SetNextItemWidth(-1);
					//		if (i >= 5)
					//			ImGui::InputFloat("##value", &dummy_members[i], 1.0f);
					//		else
					//			ImGui::DragFloat("##value", &dummy_members[i], 0.01f);
					//		ImGui::NextColumn();
					//	}
					//	ImGui::PopID();
					//}
					ImGui::TreePop();
				}
				ImGui::PopID();
			}

			static void ShowEdgeProperties(const char* prefix, Edge& smc, int uid)
			{
				ImGui::PushID(uid);
				ImGui::AlignTextToFramePadding();
				bool node_open = ImGui::TreeNode("Edge", "%s_%u", prefix, uid);
				ImGui::NextColumn();
				ImGui::AlignTextToFramePadding();
				ImGui::Text("SMC");
				ImGui::NextColumn();
				if (node_open)
				{
					ImGui::AlignTextToFramePadding();
					ImGui::TreeNodeEx("Start", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
					ImGui::NextColumn();
					ImGui::SetNextItemWidth(-1);
					ImGui::DragFloat3(" ", (float*)&smc.start, 0.1f);
					ImGui::NextColumn();
					ImGui::TreeNodeEx("End", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
					ImGui::NextColumn();
					ImGui::SetNextItemWidth(-1);
					ImGui::DragFloat3("  ", (float*)&smc.end, 0.1f);
					//new direction
					smc.SetDirection();
					ImGui::NextColumn();
					ImGui::TreeNodeEx("Classification", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
					ImGui::NextColumn();
					ImGui::RadioButton("IN", &smc.status, 1); ImGui::SameLine();
					ImGui::RadioButton("ON", &smc.status, 2); ImGui::SameLine();
					ImGui::RadioButton("OUT", &smc.status, 3);
					ImGui::NextColumn();

					ImGui::TreePop();
				}
				ImGui::PopID();
			}
		};

		for (int obj_i = 0; obj_i < objs.size(); obj_i++)
			funcs::ShowObjectProperties("Primitive", objs[obj_i], obj_i);

		for (int edge_i = 0; edge_i < objCSG.edgeList.size(); edge_i++)
			funcs::ShowEdgeProperties("Set Membership", objCSG.edgeList[edge_i], edge_i);

		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();
		ImGui::End();
	}

	// Main Menu Bar
	static void Main_Menu_Bar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				{
					if (ImGui::MenuItem("New", "N")) { New_MENU(); }
					if (ImGui::MenuItem("Open", NULL, false, false)) {}
					if (ImGui::MenuItem("Save", NULL, false, false)) {}
					ImGui::Separator();
					if (ImGui::BeginMenu("Examples..."))
					{
						if (ImGui::MenuItem("TEMA.csg")) { New_MENU(); TEMA(); Render_MENU(); }
						if (ImGui::MenuItem("Ex-1.csg")) { New_MENU(); Example_1(); Render_MENU(); }
						if (ImGui::MenuItem("Ex-2.csg")) { New_MENU(); Example_2(); Render_MENU(); }
						if (ImGui::MenuItem("Ex-3.csg")) { New_MENU(); Example_3(); Render_MENU(); }
						ImGui::EndMenu();
					}
					ImGui::Separator();
					if (ImGui::MenuItem("Properties"/*, NULL, false, false*/, "P"))
					{
						if (show_app_property_editor)
							show_app_property_editor = false;
						else show_app_property_editor = true;
					}
					if (ImGui::MenuItem("Quit")) { glutDestroyWindow(1); }
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Move", NULL, false, false)) {}
				if (ImGui::MenuItem("Scale", NULL, false, false)) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Union")) { objCSG.Union(objs); objs.back().op = 1;	}
				if (ImGui::MenuItem("Subtract")) { objCSG.Subtract(objs); objs.back().op = 2; }
				if (ImGui::MenuItem("Intersect")) { objCSG.Intersect(objs); objs.back().op = 3; }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Add"))
			{
				if (ImGui::MenuItem("Cube"))
				{
					std::cout << "CUBE" << std::endl;
					cubes.push_back(Cube()); //center, size
					objs.push_back(Object("Cubo", Neutral, new Cube()));
					objs.back().op = 1;
				}
				if (ImGui::MenuItem("Sphere"))
				{
					std::cout << "SPHERE" << std::endl;
					objs.push_back(Object("Esfera", Red, new Sphere()));
					objs.back().op = 1;
					spheres.push_back(Sphere()); //center, radius
					spheres.back().id = objs.size() - 1;
				}
				if (ImGui::MenuItem("Cylinder"))
				{
					std::cout << "CYLINDER" << std::endl;
					cylinders.push_back(Cylinder()); //height, radius, center, direction
					objs.push_back(Object("Cilindro", Green, new Cylinder()));
					objs.back().op = 1;
				}
				if (ImGui::MenuItem("Cone"))
				{
					std::cout << "CONE" << std::endl;
					cones.push_back(Cone());
					objs.push_back(Object("Cone", Blue, new Cone())); //height, radius, center, direction
					objs.back().op = 1;
				}
				ImGui::Separator();
				if (ImGui::MenuItem("EDGE"))
				{
					std::cout << "Edge" << std::endl;
					objCSG.edgeList.push_back(Edge({ -5.0f, -4.0f, -3.0f, 1.0f }, { 5.0f, 4.0f, 3.0f, 1.0f }, 3));
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Render"))
			{
				if (ImGui::MenuItem("Render Image", "R")) { Render_MENU(); }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Analysis"))
			{
				if (ImGui::MenuItem("Area", NULL, false, false)) {}
				if (ImGui::MenuItem("Volume"/*, NULL, false, false*/))
					if (show_mass_window)
						show_mass_window = false;
					else show_mass_window = true;
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

	/* ======================================= INITIALIZE ======================================= */

	void init(void)
	{
		/* LIGHT */
		
		GLfloat luzAmbiente[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // Ambient Light
		GLfloat luzDifusa[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; // "cor" 
		GLfloat luzEspecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // "brilho" 
		GLfloat especularidade[4] = { 0.1f, 0.1f, 0.1f, 1.0f }; // brilho do material
		GLint especMaterial = 100;

		glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade); // refletância do material 
		glMateriali(GL_FRONT, GL_SHININESS, especMaterial); // concentração do brilho

		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente); // Ambient Light ENABLE
		// light0 Parameters
		glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
		glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
		glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

		glEnable(GL_COLOR_MATERIAL); // definição da cor do material a partir da cor corrente
		glEnable(GL_LIGHTING); // lighting ENABLE
		glEnable(GL_LIGHT0); // light0 ENABLE
		//glEnable(GL_DEPTH_TEST); // depth-buffering ENABLE
		
		/* LIGHT END */

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(0.3, 0.3, 0.3, 0.0); // Especifica a cor de fundo da janela
		glShadeModel(GL_SMOOTH); // Habilita o modelo de colorização de Gouraud
		glEnable(GL_LINE_SMOOTH);
		/*glEnable(GL_DEPTH_TEST);*/
		//glEnable(GL_CULL_FACE); //  remove as faces cujas normais apóntam para o semiespaço oposto ao do observador
	}

	/* ======================================= glutDisplayFunc ======================================= */

	void display(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		gluLookAt(cam.eye.x, cam.eye.y, cam.eye.z, cam.lookat.x, cam.lookat.y, cam.lookat.z, cam.up.x, cam.up.y, cam.up.z);

		// grid
		glLineWidth(1.0f);
		glBegin(GL_LINES);
		for (float i = -50; i <= 50; ++i)
		{
			glColor4f(0.3f, 0.3f, 0.3f, 0.5f * (1 - abs(i) / 50));
			if (i == 0) glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
			glVertex3f(i, 0.0f, -50.0f);
			glVertex3f(i, 0.0f, 50.0f);
			if (i == 0) glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			glVertex3f(50.0f, 0.0f, i);
			glVertex3f(-50.0f, 0.0f, i);
		}
		glEnd();

		// Y-axis
		glBegin(GL_LINES);
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		glEnd();

		OpenGL_Render::DrawEdges(objCSG.edgeList);
		OpenGL_Render::DrawCubes(cubes);
		OpenGL_Render::DrawSpheres(spheres);
		OpenGL_Render::DrawCylinders(cylinders);
		OpenGL_Render::DrawCones(cones);

		// light
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		glPushMatrix();
		glTranslatef(posicaoLuz[0], posicaoLuz[1], posicaoLuz[2]);
		glutWireSphere(0.2, 20, 16); //radius, slices, stacks
		glPopMatrix();

		// ImGUI
		{
			// Start the Dear ImGui frame
			ImGui_ImplOpenGL2_NewFrame();
			ImGui_ImplGLUT_NewFrame();

			Main_Menu_Bar();
			if (show_app_property_editor) ShowAppPropertyEditor(&show_app_property_editor);
			if (show_mass_window) ShowMassPropertyWindow(&show_mass_window);

			// Rendering
			ImGui::Render();
			ImGuiIO& io = ImGui::GetIO();

			ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
		}

		//glFlush();
		glutSwapBuffers();
		glutPostRedisplay();
	}

	/* ======================================= glutReshapeFunc ======================================= */

	void reshape(int w, int h)
	{
		ImGui_ImplGLUT_ReshapeFunc(w, h);

		glViewport(0, 0, (GLsizei)w, (GLsizei)h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0f, 500.0);
		glMatrixMode(GL_MODELVIEW);
	}

	/* ======================================= glutCreateMenu ======================================= */

	void MenuModify(int op)
	{
		switch (op) {
		case 0:
			// Move
			MoveKey = true;
			ScaleKey = false;
			std::cout << "Move ON" << std::endl;
			break;
		case 1:
			// Scale
			ScaleKey = true;
			MoveKey = false;
			std::cout << "Scale ON" << std::endl;
			break;
		default:
			break;
		}
		glutPostRedisplay();
	}

	/* ======================================= glutCreateMenu ======================================= */

	void MenuEditing(int op)
	{
		switch (op) {
		case 0:
			// Union
			objCSG.Union(objs);
			objs.back().op = 1;
#if DEBUG == 1
			std::cout << "size of setMembership = " << objCSGTree.setMembership.size() << std::endl;
			for (int i = 0; i < objCSGTree.setMembership.size(); i++)
			{
				std::cout << "************************" << std::endl;
				std::cout << "i=" << i << std::endl;
				std::cout << "start=(" << objCSGTree.setMembership[i].start.x << "; " << objCSGTree.setMembership[i].start.y << "; " << objCSGTree.setMembership[i].start.z << ")" << std::endl;
				std::cout << "end=(" << objCSGTree.setMembership[i].end.x << "; " << objCSGTree.setMembership[i].end.y << "; " << objCSGTree.setMembership[i].end.z << ")" << std::endl;
				std::cout << "status = " << objCSGTree.setMembership[i].status << std::endl;
			}
#endif
			break;
		case 1:
			// Subtract
			objCSG.Subtract(objs);
			objs.back().op = 2;
			break;
		case 2:
			// Intersect
			objCSG.Intersect(objs);
			objs.back().op = 3;
			break;
		default:
			break;
		}
		glutPostRedisplay();
	}

	/* ======================================= glutCreateMenu ======================================= */

	void MenuModeling(int op)
	{
		switch (op) {
		case 0:
			//BOX
			std::cout << "CUBE" << std::endl;
			objs.push_back(Object("Cubo", Neutral, new Cube()));
			objs.back().op = 1;
			cubes.push_back(Cube()); //center, size
			cubes.back().id = objs.size() - 1;
			break;
		case 1:
			//SPHERE
			std::cout << "SPHERE" << std::endl;
			objs.push_back(Object("Esfera", Red, new Sphere()));
			objs.back().op = 1;
			spheres.push_back(Sphere()); //center, radius
			spheres.back().id = objs.size() - 1;
			break;
		case 2:
			//CYLINDER
			std::cout << "CYLINDER" << std::endl;
			objs.push_back(Object("Cilindro", Green, new Cylinder()));
			objs.back().op = 1;
			cylinders.push_back(Cylinder()); //height, radius, center, direction
			cylinders.back().id = objs.size() - 1;
			break;
		case 3:
			//CONE
			std::cout << "CONE" << std::endl;
			objs.push_back(Object("Cone", Blue, new Cone())); //height, radius, center, direction
			objs.back().op = 1;
			cones.push_back(Cone());
			cones.back().id = objs.size() - 1;
			break;
		case 4:
			//EDGE
			std::cout << "Edge" << std::endl;
			objCSG.edgeList.push_back(Edge({ -5.0f, -4.0f, -3.0f, 1.0f }, { 5.0f, 4.0f, 3.0f, 1.0f }, 3));
			break;
		default:
			break;
		}
		glutPostRedisplay();
	}

	/* ======================================= glutCreateMenu ======================================= */

	void MenuExamples(int op)
	{
		switch (op) {
		case 0:
			//TEMA
			New_MENU();
			TEMA();
			Render_MENU();
			break;
		case 1:
			//Example 1
			New_MENU();
			Example_1();
			Render_MENU();
			break;
		case 2:
			//Example 2
			New_MENU();
			Example_2();
			Render_MENU();
			break;
		case 3:
			//Example 3
			New_MENU();
			Example_3();
			Render_MENU();
			break;
		default:
			break;
		}
		glutPostRedisplay();
	}

	/* ======================================= glutCreateMenu ======================================= */

	void MenuTree(int op)
	{
		switch (op) {
		case 0:
			//LOAD TREE
			break;
		case 1:
			//EXPORT TREE
			break;
		default:
			break;
		}
		glutPostRedisplay();
	}

	/* ======================================= glutCreateMenu ======================================= */

	void MenuMassProperty(int op)
	{
		switch (op) {
		case 0:
			//VOLUME
			break;
		case 1:
			//AREA
			break;
		default:
			break;
		}
		glutPostRedisplay();
	}

	/* ======================================= glutCreateMenu ======================================= */

	void ProcessMenu(int op)
	{
		switch (op) {
		case 0: //NEW
			New_MENU();
			break;
		case 1:
			//RENDER
			Render_MENU();
			break;
		case 2:
			glutDestroyWindow(1); //exit
			break;
		default:
			break;
		}
		glutPostRedisplay();
	}

	/* ======================================= MAIN MENU ======================================= */

	void MainMenu()
	{
		int subMenu1, subMenu2, subMenu3, subMenu4, subMenu5, subMenu6;

		subMenu1 = glutCreateMenu(MenuModeling);
		glutAddMenuEntry("Cube", 0);
		glutAddMenuEntry("Sphere", 1);
		glutAddMenuEntry("Cylinder", 2);
		glutAddMenuEntry("Cone", 3);
		glutAddMenuEntry("EDGE", 4);

		subMenu2 = glutCreateMenu(MenuEditing);
		glutAddMenuEntry("Union", 0);
		glutAddMenuEntry("Subtract", 1);
		glutAddMenuEntry("Intersect", 2);

		subMenu3 = glutCreateMenu(MenuModify);
		glutAddMenuEntry("Move", 0);
		glutAddMenuEntry("Scale", 1);

		subMenu4 = glutCreateMenu(MenuMassProperty);
		glutAddMenuEntry("Volume", 0);
		glutAddMenuEntry("Area", 1);

		subMenu5 = glutCreateMenu(MenuTree);
		glutAddMenuEntry("Load", 0);
		glutAddMenuEntry("Export", 1);

		subMenu6 = glutCreateMenu(MenuExamples);
		glutAddMenuEntry("TEMA", 0);
		glutAddMenuEntry("Example 1", 1);
		glutAddMenuEntry("Example 2", 2);
		glutAddMenuEntry("Example 3", 3);

		glutCreateMenu(ProcessMenu);
		glutAddMenuEntry("NEW", 0);
		glutAddSubMenu("MODELING", subMenu1);
		glutAddSubMenu("SOLID EDITING", subMenu2);
		glutAddSubMenu("MODIFY", subMenu3);
		glutAddSubMenu("MASS PROPERTY", subMenu4);
		glutAddSubMenu("TREE", subMenu5);
		glutAddSubMenu("EXAMPLES", subMenu6);
		glutAddMenuEntry("RENDER", 1);
		glutAddMenuEntry("EXIT", 2);
		glutAttachMenu(GLUT_RIGHT_BUTTON);
	}

	/* ======================================= glutMouseFunc ======================================= */

	/* Get mouse position on screen */
	void mouse(int button, int state, int x, int y)
	{
		ImGui_ImplGLUT_MouseFunc(button, state, x, y);

		if (!(ImGui::GetIO().WantCaptureMouse))
		{
			if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
			{
				last = { (float)x, (float)y, 0.0f, 0.0f };
				TheButtonState = GLUT_MIDDLE_BUTTON;
			}
			if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
			{
				TheButtonState = 0;
			}
			if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
			{
				MainMenu();
			}
			if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
			{
				last = { (float)x, (float)y, 0.0f, 1.0f };
				TheButtonState = GLUT_LEFT_BUTTON;
			}
			if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
			{
				TheButtonState = 0;
			}
			glutPostRedisplay();
		}
	}

	/* ======================================= glutMotionFunc ======================================= */

	void mouseMotion(int x, int y)
	{
		ImGui_ImplGLUT_MotionFunc(x, y);

		Vector4Df delta = { last.x - (float)x, (last.y - (float)y)*(-1.0f), 0.0f, 0.0f };
		Matrix4 mToCamera = ToCamera(cam);
		Matrix4 mToWorld = ToWorld(cam);

		if (!(ImGui::GetIO().WantCaptureMouse))
		{
			if (TheButtonState == GLUT_MIDDLE_BUTTON)
			{
				/* 3D ORBIT: Rotates the view in 3D space */
				if (TheKeyState == GLUT_KEY_CTRL_L)
				{
					Vector4Df eyeToCam = mToCamera * cam.eye; // change to camera
					eyeToCam += delta * (0.2f); // motion
					cam.eye = mToWorld * eyeToCam; // change back to world
				}
				/* PAN: Shifts the view without changing the viewing direction or magnification */
				if (TheKeyState == GLUT_KEY_SHIFT_L)
				{
					// change to camera
					Vector4Df eyeToCam = mToCamera * cam.eye;
					Vector4Df lookatToCam = mToCamera * cam.lookat;
					// motion
					eyeToCam += delta * panSpeed;
					lookatToCam += delta * panSpeed;
					// change back to world
					cam.eye = mToWorld * eyeToCam;
					cam.lookat = mToWorld * lookatToCam;
				}
			}

			if (TheButtonState == GLUT_LEFT_BUTTON)
			{
				/* Translate */
				if (MoveKey == true)
				{

				}
			}

			last = { (float)x, (float)y, 0.0f, 1.0f };
			glutPostRedisplay();
		}
	}

	/* ======================================= glutMouseWheelFunc ======================================= */

	/* ZOOM: Increases or decreases the magnification of the view in the current viewport */
	void mouseWheel(int button, int dir, int x, int y)
	{
		Vector4Df lookat = cam.lookat - cam.eye;
		//lookat.normalize();
		cam.eye += lookat * (dir * 0.1f);
		//cam.lookat += lookat*dir;
		glutPostRedisplay();
	}

	/* ======================================= glutSpecialFunc ======================================= */

	void specialKey(int key, int x, int y)
	{
		switch (key) {
		case GLUT_KEY_F5: // iso view
			cam.eye = { 6.0f, 6.0f, 10.0f, 1.0f };
			cam.lookat = { 0.0f, 0.0f, 0.0f, 1.0f };
			std::cout << "ISO VIEW" << std::endl;
			break;
		case GLUT_KEY_F6: // front view
			cam.eye = { 0.0f, 0.0f, 10.0f, 1.0f };
			cam.lookat = { 0.0f, 0.0f, 0.0f, 1.0f };
			std::cout << "FRONT VIEW" << std::endl;
			break;
		case GLUT_KEY_F7: // top view
			cam.eye = { 0.0f, 10.0f, 0.1f, 1.0f };
			cam.lookat = { 0.0f, 0.0f, 0.0f, 1.0f };
			std::cout << "TOP VIEW" << std::endl;
			break;
		case GLUT_KEY_CTRL_L: // active ROTATE
			TheKeyState = key;
			std::cout << "3D Orbit ON" << std::endl;
			break;
		case GLUT_KEY_SHIFT_L: // active PAN
			TheKeyState = key;
			std::cout << "Pan ON" << std::endl;
			break;
		case GLUT_KEY_UP: // scale and rotate UP
			if (ScaleKey == true)
			{
				
			}
			break;
		case GLUT_KEY_DOWN: // scale and rotate DOWN
			if (ScaleKey == true)
			{
				
			}
			break;
		case GLUT_KEY_HOME:
			break;
		default:
			break;
		}
		glutPostRedisplay();
	}

	/* ======================================= glutKeyboardFunc ======================================= */

	void keyboard(unsigned char key, int x, int y)
	{
		switch (key) {
		case 'M':
		case 'm':
			// Move
			MoveKey = true;
			ScaleKey = false;
			std::cout << "Move ON" << std::endl;
			break;
		case 'N':
		case 'n':
			// NEW
			New_MENU();
			break;
		case 'P':
		case 'p':
			// Properties
			if (show_app_property_editor)
				show_app_property_editor = false;
			else show_app_property_editor = true;
			break;
		case 'R':
		case 'r':
			// RENDER
			Render_MENU();
			break;
		case 'S': // change select octree
		case 's':
			/*if (index == objOctree.treeList.size() - 1)
				index = 0;
			else index++;*/
			break;
		}
		glutPostRedisplay();
	}

	/* ======================================= MAIN ======================================= */

	int Start_Window(int argc, char** argv)
	{
		RenderAPI::cam = cam;
		RenderAPI::height = heightRender;
		RenderAPI::objs = objs;
		RenderAPI::width = widthRender;

		//window 1
		glutInit(&argc, argv);
#ifdef __FREEGLUT_EXT_H__
		glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glutInitWindowSize(600, 600);
		glutInitWindowPosition(550, 0);
		glutCreateWindow("CSG");
		init();

		// ImGUI
		{
			// Setup Dear ImGui context
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

			// Setup Dear ImGui style
			//ImGui::StyleColorsDark();
			ImGui::StyleColorsClassic();

			// Setup Platform/Renderer bindings
			ImGui_ImplGLUT_Init();
			ImGui_ImplGLUT_InstallFuncs();
			ImGui_ImplOpenGL2_Init();
		}

		glutDisplayFunc(display);
		glutReshapeFunc(reshape);
		glutMouseFunc(mouse);
		glutMotionFunc(mouseMotion);
		glutMouseWheelFunc(mouseWheel);
		glutSpecialFunc(specialKey);
		glutKeyboardFunc(keyboard);
		glEnable(GL_DEPTH_TEST);
		
		//window 2
		glutInitWindowPosition(0, 0);
		glutInitWindowSize(widthRender, heightRender);
		glutCreateWindow("CG");

		glClearColor(0.0, 1.0, 0.0, 0.0);
		glMatrixMode(GL_PROJECTION);
		gluOrtho2D(0.0, widthRender, 0.0, heightRender);

		glewInit();
		if (!glewIsSupported("GL_VERSION_2_0 ")) {
			exit(0);
		}

		RenderAPI::init();
		glutDisplayFunc(RenderAPI::disp);
		glutReshapeFunc(RenderAPI::reshape);
		RenderAPI::CreateVBO(&RenderAPI::vbo, widthRender, heightRender);

		glutMainLoop();

		// ImGUI
		{
			// Cleanup
			ImGui_ImplOpenGL2_Shutdown();
			ImGui_ImplGLUT_Shutdown();
			ImGui::DestroyContext();
		}

		return 0;
	}
}
