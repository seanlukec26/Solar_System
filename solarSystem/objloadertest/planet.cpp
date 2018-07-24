#include "Planet.h"

Planet::Planet(float angleparam, float scaleparam, float tiltparam, float sunangleparam, float translateparam)
{
	planet = mat4(1.0f);

	planetangle = 0.0f;
	planetangle += angleparam;

	planetscale = scale(planetscale, vec3(scaleparam, scaleparam, scaleparam));

	planettilt = rotate(planettilt, tiltparam, vec3(0.0f, 0.0f, 1.0f));

	sunangle = 0.0f;
	sunangle += sunangleparam;

	planettranslate = translate(planettranslate, vec3(translateparam, 0.0f, 0.0f));

	sunrotate = planetangle;
}

mat4 Planet::getPlanet() const
{
	return planet;
}

void Planet::updatePlanet()
{
	sunrotation = rotate(sunrotation, sunrotate, vec3(0.0f, 1.0f, 0.0f));

	planetrotate = rotate(planetrotate, sunangle, vec3(1.0f, 1.0f, 0.32f));

	planet = sunrotation * planettranslate * planetscale * planettilt * planetrotate;

}
