#include "AmbientLight.h"

AmbientLight::AmbientLight()
{

}

AmbientLight::AmbientLight(Vector4Df intensity) : Light (intensity)
{
}

AmbientLight::~AmbientLight()
{

}

Vector4Df AmbientLight::Illumination(const Material &mat, const Vector4Df &normal, const Vector4Df &view, const Vector4Df &P){
	Vector4Df color;

	color = this->intensity * mat.ka;

	return color;
}