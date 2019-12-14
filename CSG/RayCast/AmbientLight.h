#pragma once

#include "Light.h"

class AmbientLight : public Light
{

public:
	AmbientLight();
	AmbientLight(Vector4Df intensity);
	~AmbientLight();

	Vector4Df Illumination(const Material &mat, const Vector4Df &normal, const Vector4Df &view, const Vector4Df &P);
};

