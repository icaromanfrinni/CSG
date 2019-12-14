#pragma once
#include "Camera.h"
#include "Object.h"
#include "Light.h"
#include "CSG.h"

//Class to render using raycast

class RayCast {
	float2 resolution;
	Vector4Df *accumulateBuffer;

public:

	RayCast();
	RayCast(const Camera &cam);

	~RayCast();

	//return a Vector4df matrix where each Vector4Df is a color
	Vector4Df* Render(const Camera &cam, const std::vector<Object> &objects, std::vector<Light *> lights);

	Object* RayPick(const Camera &cam, std::vector<Object> &objects, int x, int y);

};