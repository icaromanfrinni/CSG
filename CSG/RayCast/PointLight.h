#pragma once

#include "Light.h"

class PointLight : public Light
{
	Vector4Df position;  // light position
public:
	PointLight(Vector4Df intensity, Vector4Df _position);
	~PointLight();

	Vector4Df Illumination(const Material &mat, const Vector4Df &normal, const Vector4Df &view, const Vector4Df &P);
};