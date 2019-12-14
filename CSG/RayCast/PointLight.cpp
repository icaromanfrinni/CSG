#include "PointLight.h"

PointLight::PointLight(Vector4Df intensity, Vector4Df _position) : Light(intensity), position(_position) {}

PointLight::~PointLight() {}

Vector4Df PointLight::Illumination(const Material& mat, const Vector4Df& normal, const Vector4Df& view, const Vector4Df& P)
{
	// light vector
	Vector4Df L = position - P;
	L.normalize();

	Vector4Df Ia = intensity * mat.ka;                                   // ambient reflection
	Vector4Df Id = { 0.0f, 0.0f, 0.0f, 0.0f };			           // diffuse reflection
	Vector4Df Is = { 0.0f, 0.0f, 0.0f, 0.0f };					  // specular reflection

	float dot_d_n = dot(L, normal);
	if (dot_d_n > 0.0f) {
		Id = (intensity * mat.kd) * dot_d_n; //diffuse reflection
	}
	const Vector4Df r = reflection(L, normal);
	float dot_r_v = dot(r, view);
	if (dot_r_v > 0.0f) {
		Is = (intensity * mat.ks) * powf(dot_r_v, mat.alfa);//specular reflection
	}

	return Ia + Id + Is;
}