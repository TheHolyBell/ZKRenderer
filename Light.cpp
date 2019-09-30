#include "Light.h"

void Light::SetPosition(float x, float y, float z)
{
	Pos.x = x;
	Pos.y = y;
	Pos.z = z;
}

void Light::SetAmbient(float r, float g, float b)
{
	Ambient.x = r;
	Ambient.y = g;
	Ambient.z = b;
}

void Light::SetDiffuse(float r, float g, float b)
{
	Diffuse.x = r;
	Diffuse.y = g;
	Diffuse.z = b;
}
