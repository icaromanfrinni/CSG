#include "Material.h"

Material::Material()
{

}

Material::Material(Vector4Df ka, Vector4Df kd, Vector4Df ks, float alfa)
{
	this->ka = ka;
	this->kd = kd;
	this->ks = ks;
	this->alfa = alfa;
}

Material::~Material()
{

}