#pragma once
#include "Linear_Algebra.h"

class Material
{
public:
	Vector4Df ka; // Ambient Coefficient
	Vector4Df kd; // Diffuse Coefficient
	Vector4Df ks; // Spectral Coefficient
	float alfa;   // Shininess

	Material();
	Material(Vector4Df ka, Vector4Df kd, Vector4Df ks, float alfa);
	~Material();
};

