#ifndef PLANET_H
#define PLANET_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Windows.h>
#include <string>
#include "aitmesh.h"

using namespace std;
using namespace glm;

class Planet
{
public:
	Planet(float angleparam, float scaleparam, float tiltparam, float sunangleparam, float translateparam);
	
	mat4 getPlanet() const;
	void updatePlanet();

private:
	float planetangle;
	mat4 planetscale;
	mat4 planettilt;
	mat4 planetrotate;
	float sunangle;
	mat4 planettranslate;
	float sunrotate;
	mat4 sunrotation;
	mat4 planet;

};
#endif
