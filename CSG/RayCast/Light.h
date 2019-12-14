#pragma once
#include "Linear_Algebra.h"
#include "Material.h"

class Light
{
public:
	Vector4Df intensity;

	Light();
	Light(Vector4Df intensity);
	virtual ~Light() = 0;

	//Return the collor after the interation with the light and material
	virtual Vector4Df Illumination(const Material &mat, const Vector4Df &normal, const Vector4Df &view, const Vector4Df &P) = 0;
};