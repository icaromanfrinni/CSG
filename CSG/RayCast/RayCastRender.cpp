#include "RayCastRender.h"

#include <iostream>
#include <time.h>
#include <omp.h>

#define PRINT 0 //set if will print the colisions on terminal or not
#define DEBUG 1

//#define X_PRINT 256
//#define Y_PRINT 256


RayCast::RayCast() {
}

RayCast::RayCast(const Camera &cam) : resolution(cam.resolution) {

	accumulateBuffer = new(nothrow) Vector4Df[resolution.x * resolution.y * sizeof(Vector4Df)];

	for (unsigned y = 0, s = (unsigned)(resolution.x * resolution.y); y < s; y++) {
		accumulateBuffer[y] = Vector4Df{ 0.5f, 0.5f, 0.5f, 0.5f };
	}
}

RayCast::~RayCast() {
	delete[]accumulateBuffer;
}

inline Vector4Df ray_cast(register const Ray &ray, const std::vector<Object> &objects, const std::vector<Light *> &lights, bool print, float cam_near)
{
	float dist = INFINITY;

	// background color
	Vector4Df accucolor = Vector4Df{ 0.0f, 0.0f, 0.0f, 0.0f };

#if PRINT == 1
	int id = 0;
#endif

	// Initialize SetMembership
	setMembership setRay(Edge(ray.origin, ray.direction * INFINITY, 3));

	//for (const Object &obj : objects)
	for (int i = 0; i < objects.size(); i++)
	{
		// collision list
		RayCollisionList dist_List = objects[i].CollideAll(ray);

		Edge currentMembership;
		// get the current object
		if (dist_List.collisions.size() == 2)
		{
			currentMembership = Edge(dist_List.collisions[0].point, dist_List.collisions[1].point, 1);
			currentMembership.obj = objects[i];
		}
		else
		{
			currentMembership = Edge(ray.origin, ray.direction * INFINITY, 3);
		}

		/* ÁRVORE CSG */
		if (objects[i].op == 1)
			setRay.Union(currentMembership);
		else if (objects[i].op == 2)
			setRay.Subtract(currentMembership);
		else if (objects[i].op == 3)
			setRay.Intersect(currentMembership);

#if DEBUG == 2
		if (setRay.edges.size() == 3)
		{
			std::cout << "===========================" << std::endl;
			std::cout << "setRay size = " << setRay.edges.size() << std::endl;
			std::cout << "\n" << std::endl;

			if (setRay.edges[1].status == 3)
			for (int i = 0; i < setRay.edges.size(); i++)
			{
				std::cout << "i = " << i << std::endl;
				std::cout << "status = " << setRay.edges[i].status << std::endl;
				std::cout << "start = (" << setRay.edges[i].start.x << "; " << setRay.edges[i].start.y << "; " << setRay.edges[i].start.z << ")" << std::endl;
				std::cout << "end = (" << setRay.edges[i].end.x << "; " << setRay.edges[i].end.y << "; " << setRay.edges[i].end.z << ")" << std::endl;
				if (setRay.edges[i].status == 1) std::cout << typeid(*setRay.edges[i].obj.getGeometry()).name() << std::endl;
				std::cout << "\n\n" << std::endl;

				system("pause");
			}
		}
#endif

		/* RENDER */

		//const float o_dist = (setRay.edges[0].end - setRay.edges[0].start).length();
		// pegar o primeiro segmento "vivo"
		float o_dist = INFINITY;
		for (int j = 0; j < setRay.edges.size(); j++)
			if (setRay.edges[j].status == 1)
			{
				o_dist = (setRay.edges[j].start - setRay.edges[0].start).length();
				//break;

				if (/*o_dist < dist &&*/ o_dist > cam_near)
				{
					accucolor = Vector4Df{ 0.0f, 0.0f, 0.0f, 0.0f };
					dist = o_dist;
					Vector4Df pS = ray.origin + (ray.direction * dist); // Surface Point
					for (Light * light : lights)
					{
						//accucolor += light->Illumination((*obj.getMaterial()), obj.getNormalVector(pS), ray.direction * (-1.0f), pS);
						accucolor += light->Illumination((*setRay.edges[j].obj.getMaterial()), setRay.edges[j].obj.getNormalVector(pS), ray.direction * (-1.0f), pS);
					}
					break; //pula pra próx objeto
				}
			}
			else if (j == setRay.edges.size() - 1)
			{
				accucolor = Vector4Df{ 0.0f, 0.0f, 0.0f, 0.0f };
			}
			
		/*
		if (o_dist < dist && o_dist > cam_near)
		{
			accucolor = Vector4Df{ 0.0f, 0.0f, 0.0f, 0.0f };
			dist = o_dist;
			Vector4Df pS = ray.origin + (ray.direction * dist); // Surface Point
			for (Light * light : lights)
			{
				//accucolor += light->Illumination((*obj.getMaterial()), obj.getNormalVector(pS), ray.direction * (-1.0f), pS);
				accucolor += light->Illumination((*setRay.edges[1].obj.getMaterial()), setRay.edges[1].obj.getNormalVector(pS), ray.direction * (-1.0f), pS);
			}
			//break; //pula pra próx objeto
		}
		*/

		/* AS DUAS COLISÕES */

		//for (int i = 0; i < dist_List.collisions.size(); i++)
		//{
		//	const float o_dist = dist_List.collisions[i].t;

		//	if (o_dist < dist && o_dist > cam_near)
		//	{
		//		accucolor = Vector4Df{ 0.0f, 0.0f, 0.0f, 0.0f };
		//		//TODO remove this visible after
		//		//if (obj.visible) { dist = o_dist; /* accucolor = obj.getMaterial()->ka;*//*accucolor = Vector4Df{ 1.0f, 0.0f, 0.0f, 0.0f };*/ }
		//		dist = o_dist;
		//		Vector4Df pS = ray.origin + (ray.direction * dist); // Surface Point
		//		for (Light * light : lights)
		//		{
		//			//accucolor += light->Illumination((*obj.getMaterial()), Vector4Df{ 0.0f, 0.0f, 0.0f, 0.0f }, Vector4Df{ 0.0f, 0.0f, 0.0f, 0.0f });
		//			accucolor += light->Illumination((*obj.getMaterial()), obj.getNormalVector(pS), ray.direction * (-1.0f), pS);
		//		}
		//		break; //pula pra próx objeto
		//	}
		//}
		
		/* APENAS O MAIS PRÓXIMO */

		//const float o_dist = obj.Collide(ray);
		//if (o_dist < dist && o_dist > cam_near) {
		//	accucolor = Vector4Df{ 0.0f, 0.0f, 0.0f, 0.0f };
		//	//TODO remove this visible after
		//	//if (obj.visible) { dist = o_dist; /* accucolor = obj.getMaterial()->ka;*//*accucolor = Vector4Df{ 1.0f, 0.0f, 0.0f, 0.0f };*/ }
		//	dist = o_dist;
		//	Vector4Df pS = ray.origin + (ray.direction * dist); // Surface Point
		//	for (Light * light : lights)
		//	{
		//		//accucolor += light->Illumination((*obj.getMaterial()), Vector4Df{ 0.0f, 0.0f, 0.0f, 0.0f }, Vector4Df{ 0.0f, 0.0f, 0.0f, 0.0f });
		//		accucolor += light->Illumination((*obj.getMaterial()), obj.getNormalVector(pS), ray.direction * (-1.0f), pS);
		//	}

#if PRINT == 1
		if (print) {
			RayCollisionList cols = obj.CollideAll(ray);
			std::cout << "-- Colisoes com :" << id << " " << typeid(*obj.getGeometry()).name() << "\n";
			for (Collision c : cols.collisions) {
				std::cout << "    t :" << c.distance << "; ";
				std::cout << "    p :" << c.pint.x << " " << c.pint.y << " " << c.pint.z << "\n";
			}
		}
		id++;
#endif
	}

	return accucolor;
}

//will be removed after
Object* RayCast::RayPick(const Camera &cam, std::vector<Object> &objects, int x, int y) {
	const Vector4Df base = (cam.lookat - cam.eye).to_unitary();
	const Vector4Df up = cam.up * (cam.dimensions.y / cam.resolution.y);
	const Vector4Df left = cross(cam.up, base) * (cam.dimensions.x / cam.resolution.x);

	const int width = (int)cam.resolution.x;
	const int height = (int)cam.resolution.y;

	bool print = false;

	const Vector4Df posi_pix_0_0 = base * cam.n + up * (height*(0.5f) - 0.5f) + left * (width*(0.5f) - 0.5f);
	Vector4Df direction = posi_pix_0_0 + up * (-y) + left * (-x);
	direction.normalize();

	std::cout << "*** Testando *** x:" << x << " y:" << y << "\n";

	Ray ray{ cam.eye, direction };

	int id = 0;
	Object *colidiu = nullptr;
	float dist = INFINITY;
	Matrix4 to_cam = ToCamera(cam);
	for (Object &obj : objects) {
		const float o_dist = obj.Collide(ray);
		if (o_dist < dist && o_dist > cam.n) {
			dist = o_dist;
			colidiu = &obj;
		}
		RayCollisionList cols = obj.CollideAll(ray);
		//std::cout << "-- Colisoes com :" << id << " " << typeid(*obj.getGeometry()).name() << "\n";
		std::cout << "-- Colisoes com: " << obj.getName() << " (" << typeid(*obj.getGeometry()).name() << ")\n";
		for (Collision c : cols.collisions) {
			c.point = to_cam * c.point;
			std::cout << "    t :" << c.t << "; ";
			std::cout << "    p :" << c.point.x << " " << c.point.y << " " << c.point.z << "\n";
		}
		id++;
	}
	return colidiu;

}

Vector4Df* RayCast::Render(const Camera &cam, const std::vector<Object> &objects, std::vector<Light*> lights) {
	clock_t t;
	t = clock();

	const Vector4Df base = (cam.lookat - cam.eye).to_unitary();
	const Vector4Df up = cam.up * (cam.dimensions.y / cam.resolution.y);
	const Vector4Df left = cross(cam.up, base) * (cam.dimensions.x / cam.resolution.x);


	//Vector4Df *pix = accumulatebuffer;
	const int width = (int)cam.resolution.x;
	const int height = (int)cam.resolution.y;

	bool print = false;

	const Vector4Df posi_pix_0_0 = base * cam.n + up * (height*(-0.5f) + 0.5f) + left * (width*(0.5f) - 0.5f);

#pragma omp parallel for num_threads(16) schedule(guided)
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Vector4Df direction = posi_pix_0_0 + up * (y)+left * (-x);
			direction.normalize();

#if PRINT == 1
			if (x == X_PRINT && y == Y_PRINT) {
				std::cout << "*** Testando *** x:" << x << " y:" << y << "\n";
				print = true;
			}
			else {
				print = false;
			}
#endif

			accumulateBuffer[y*width + x] = ray_cast(Ray{ cam.eye, direction }, objects, lights, print, cam.n);

		}
	}

	t = clock() - t;
	//std::cout << "levou " << t << " clocks ou " << ((float)t) / CLOCKS_PER_SEC << " segundos ou " << 1.0f / (((float)t) / CLOCKS_PER_SEC) << " fps\n";


	return accumulateBuffer;
}