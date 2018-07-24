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
#include "planet.h"

using namespace std;
using namespace glm;

const int windowWidth = 1024; 
const int windowHeight = 768;

GLuint VBO;
int NUMVERTS = 0;
float zaxis = 70.0f;
float xaxis = 0.0f;
float yaxis = 0.0f;
float sunradius;

// Transform uniforms location
GLuint gModelToWorldTransformLoc;
GLuint gWorldToViewToProjectionTransformLoc;

// Lighting uniforms location
GLuint gAmbientLightIntensityLoc;
GLuint gDirectionalLightIntensityLoc;
GLuint gDirectionalLightDirectionLoc;

// Materials uniform location
GLuint gKaLoc;
GLuint gKdLoc;
GLuint gColourLoc;


static void renderSceneCallBack()
{
	Sleep(20);
	// Clear the back buffer and the z-buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Create our world space to view space transformation matrix
	mat4 worldToViewTransform = lookAt(
		vec3(xaxis, yaxis, zaxis), // The position of your camera, in world space
		vec3(xaxis,0.0f,0.0f), // where you want to look at, in world space
		vec3(0.0f,1.0f,0.0f)  // Camera up direction (set to 0,-1,0 to look upside-down)
		); 

    // Create out projection transform
    mat4 projectionTransform = perspective(45.0f, (float)windowWidth / (float)windowHeight, 1.0f, 2000.0f);

	// Combine the world space to view space transformation matrix and the projection transformation matrix
	mat4 worldToViewToProjectionTransform = projectionTransform * worldToViewTransform;

    // Update the transforms in the shader program on the GPU
    glUniformMatrix4fv(gWorldToViewToProjectionTransformLoc, 1, GL_FALSE, &worldToViewToProjectionTransform[0][0]);

    glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(aitVertex), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(aitVertex), (const GLvoid*)12);

    // Set the material properties
    glUniform1f(gKaLoc, 0.8f); 
    glUniform1f(gKdLoc, 0.8f);

	//sun
	mat4 modelToWorldTransform = mat4(1.0f);
	static float angle = 0.0f;
	angle+=13.7195121951f;

	mat4 sunscal = mat4(1.0f);
	sunscal = scale(sunscal, vec3(13.77648f, 13.77648f, 13.77648f));
	sunradius = 13.77648f;

	modelToWorldTransform = rotate(modelToWorldTransform, angle, vec3(0.0f, 1.0f, 0.0f)) * sunscal;

	glUniformMatrix4fv(gModelToWorldTransformLoc, 1, GL_FALSE, &modelToWorldTransform[0][0]);
	//colour
	glUniform4f(gColourLoc, 1.0f, 0.58039215686f, 0.0f,1.0f);
	glDrawArrays(GL_TRIANGLES, 0, NUMVERTS);  
	//end of sun 

	//mercury
	static Planet mercury = Planet(4.09090909091f, 00.04880f, -0.1f, 1.0f, sunradius + (5.79f/3));
	mercury.updatePlanet();
	modelToWorldTransform = mercury.getPlanet();
	glUniformMatrix4fv(gModelToWorldTransformLoc, 1, GL_FALSE, &modelToWorldTransform[0][0]);
	//colour
	glUniform4f(gColourLoc, 0.87843137254f, 0.66274509803f, 0.23529411764f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, NUMVERTS);
	//end of mercury

	//venus
	static Planet venus = Planet(1.60213618158f, 00.12104f, 177.0f, 1.0f, sunradius + (10.82f / 3));
	venus.updatePlanet();
	modelToWorldTransform = venus.getPlanet();
	glUniformMatrix4fv(gModelToWorldTransformLoc, 1, GL_FALSE, &modelToWorldTransform[0][0]);
	//colour
	glUniform4f(gColourLoc, 0.70980392156f, 0.63921568627f, 0.50196078431f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, NUMVERTS);
	//end of venus

	//earth
	mat4 earth = mat4(1.0f);
	static float earthangle = 0.0f;
	earthangle += 0.98576122672f; //how long it takes to rotate around the sun. 360degrees/days

	mat4 scal = mat4(1.0f);
	scal = scale(scal, vec3(0.12756f, 0.12756f, 0.12756f));

	mat4 earthtilt = mat4(1.0f);
	earthtilt = rotate(earthtilt, -23.0f, vec3(0.0f, 0.0f, 1.0f));

	mat4 earthrotate = mat4(1.0f);
	static float sunangle = 0.0f;
	sunangle += 1.0f;
	earthrotate = rotate(earthrotate, sunangle, vec3(1.0f, 1.0f, 0.32f));
	float earthradius = 0.12756f;
	mat4 trans = mat4(1.0f);
	trans = translate(trans, vec3(sunradius + (14.96f / 3), 0.0f, 0.0f));
	mat4 earthSunRot = mat4(1.0f);
	earthSunRot = rotate(earthSunRot, earthangle, vec3(0.0f, 1.0f, 0.0f));
	earth = earthSunRot * trans * scal * earthtilt * earthrotate;
	glUniformMatrix4fv(gModelToWorldTransformLoc, 1, GL_FALSE, &earth[0][0]);
	//colour
	glUniform4f(gColourLoc, 0.16862745098f, 0.30980392156f, 0.65882352941f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, NUMVERTS);
	//end earth

	//moon
	mat4 moon = mat4(1.0f);

	mat4 moonrotate = mat4(1.0f);
	static float angle2 = 0.0f;
	angle2 += 13.17715959f;
	moonrotate = rotate(moonrotate, angle2, vec3(.0f, 1.0f, .0f));
	mat4 scal2 = mat4(1.0f);
	scal2 = scale(scal2, vec3(00.03476f, 00.03476f, 00.03476f));
	mat4 trans1 = mat4(1.0f);
	trans1 = translate(trans1, vec3(earthradius + 0.3f, 0.0f, 0.0f));
	moon = earthSunRot * trans * earthtilt * earthrotate * moonrotate * trans1 * scal2;

	glUniformMatrix4fv(gModelToWorldTransformLoc, 1, GL_FALSE, &moon[0][0]);
	//colour
	glUniform4f(gColourLoc, 1.0f, 1.0f, 1.0f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, NUMVERTS);
	//end moon

	//mars
	static Planet mars = Planet(0.52401746724f, 00.06788f, -25.0f, 1.0f, sunradius + (22.79f / 3));
	//Planet(angleparam, scaleparam, tiltparam, sunangleparam, translateparam, sunrotateparam);
	mars.updatePlanet();
	modelToWorldTransform = mars.getPlanet();
	glUniformMatrix4fv(gModelToWorldTransformLoc, 1, GL_FALSE, &modelToWorldTransform[0][0]);
	//colour
	glUniform4f(gColourLoc, 0.87843137254f, 0.73725490196f, 0.40392156862f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, NUMVERTS);
	//end of mars

	//jupiter
	static Planet jupiter = Planet(0.08310249307f, 1.42740f, -3.0f, 1.0f, sunradius + (77.83f / 3));
	//Planet(angleparam, scaleparam, tiltparam, sunangleparam, translateparam, sunrotateparam);
	jupiter.updatePlanet();
	modelToWorldTransform = jupiter.getPlanet();
	glUniformMatrix4fv(gModelToWorldTransformLoc, 1, GL_FALSE, &modelToWorldTransform[0][0]);
	//colour
	glUniform4f(gColourLoc, 0.9294117647f, 0.7294117647f, 0.45490196078f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, NUMVERTS);
	//end of jupiter

	//saturn
	static Planet saturn = Planet(0.03345724907f, 1.20034f, -27.0f, 1.0f, sunradius + (142.70f / 3));
	//Planet(angleparam, scaleparam, tiltparam, sunangleparam, translateparam, sunrotateparam);
	saturn.updatePlanet();
	modelToWorldTransform = saturn.getPlanet();
	glUniformMatrix4fv(gModelToWorldTransformLoc, 1, GL_FALSE, &modelToWorldTransform[0][0]);
	//colour
	glUniform4f(gColourLoc, 0.87843137254f, 0.70588235294f, 0.30588235294f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, NUMVERTS);
	//end of saturn

	//saturnring
	mat4 saturnring = mat4(1.0f);

	mat4 saturnringscal = mat4(1.0f);
	saturnringscal = scale(saturnringscal, vec3(1.42740f+0.60300f, -0.4f, 1.42740f+0.60300f));
	mat4 saturntrans = mat4(1.0f);
	saturntrans = translate(saturntrans, vec3(sunradius + (142.70f / 3), 0.0f, 0.0f));
	mat4 saturntilt = mat4(1.0f);
	saturntilt = rotate(saturntilt, -27.0f, vec3(0.0f, 0.0f, 1.0f));
	mat4 saturnrotate = mat4(1.0f);
	saturnrotate = rotate(saturnrotate, sunangle, vec3(2.0f, 1.0f, 0.0f));
	static float saturnangle = 0.0f;
	saturnangle += 0.03345724907;

	saturnring = rotate(saturnring, saturnangle, vec3(0.0f, 1.0f, 0.0f)) * saturntrans * saturnringscal * saturntilt * saturnrotate;

	glUniformMatrix4fv(gModelToWorldTransformLoc, 1, GL_FALSE, &saturnring[0][0]);
	glUniform4f(gColourLoc, 0.66666666666f, 0.54901960784f, 0.26666666666f, 1.0f);

	glDrawArrays(GL_TRIANGLES, 0, NUMVERTS);
	//end saturnring

	//uranus
	static Planet uranus = Planet(0.01172638436f, 0.51152f, -98.0f, 1.0f, sunradius + (287.10f / 3));
	//Planet(angleparam, scaleparam, tiltparam, sunangleparam, translateparam, sunrotateparam);
	uranus.updatePlanet();
	modelToWorldTransform = uranus.getPlanet();
	glUniformMatrix4fv(gModelToWorldTransformLoc, 1, GL_FALSE, &modelToWorldTransform[0][0]);
	//colour
	glUniform4f(gColourLoc, 0.58039215686f, 0.81960784313f, 0.90980392156f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, NUMVERTS);
	//end of uranus

	//neptune
	static Planet neptune = Planet(0.00598006644f, 0.49620f, -30.0f, 1.0f, sunradius + (449.71f / 3));
	//Planet(angleparam, scaleparam, tiltparam, sunangleparam, translateparam);
	neptune.updatePlanet();
	modelToWorldTransform = neptune.getPlanet();
	glUniformMatrix4fv(gModelToWorldTransformLoc, 1, GL_FALSE, &modelToWorldTransform[0][0]);
	//colour
	glUniform4f(gColourLoc, 0.43529411764f, 0.67843137254f, 0.76862745098f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, NUMVERTS);
	//end of neptune

	//pluto
	static Planet pluto = Planet(0.00397350993f, 0.02296f, -120.0f, 1.0f, sunradius + (591.3f / 3));
	//Planet(angleparam, scaleparam, tiltparam, sunangleparam, translateparam);
	pluto.updatePlanet();
	modelToWorldTransform = pluto.getPlanet();
	glUniformMatrix4fv(gModelToWorldTransformLoc, 1, GL_FALSE, &modelToWorldTransform[0][0]);
	//colour
	glUniform4f(gColourLoc, 0.38039215686f, 0.54901960784f, 0.60784313725f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, NUMVERTS);
	//end of pluto

    glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

    glutSwapBuffers();
}

static void myKeyboard(unsigned char Key, int x, int y)
{
	switch (Key) {
		case 'w' : //zoomout
			zaxis+=0.5F;
			break;
		case 's' : //zoomin
			zaxis-=0.5f;
			break;
		case 'a': //moveleft
				xaxis -= 0.5F;
			break;
		case 'd': //moveright
				xaxis += 0.5f;
			break;
		case 'z': //moveover
				yaxis += 0.5f;
				break;
		case 'x': //moveunder
				yaxis -= 0.5f;
				break;
	}

}

static void initializeGlutCallbacks()
{
    glutDisplayFunc(renderSceneCallBack);
    glutIdleFunc(renderSceneCallBack);
}

static void createVertexBuffer()
{
	aitMesh mesh;
	if(!mesh.loadFromObj("assets/sphere.obj"))
	{
		cerr<<"Error loading mesh from obj file."<<endl;
		system("pause");
		exit(0);
	}
	vector<aitVertex> verts = mesh.getVertices();
	NUMVERTS = verts.size(); 
    
 	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(aitVertex) * NUMVERTS, &verts[0], GL_STATIC_DRAW);
}

static void initLights()
{
	// Setup the ambient light
	vec3 ambientLightIntensity = vec3(0.8f, 0.5f, 0.5f);
	glUniform3fv(gAmbientLightIntensityLoc, 1, &ambientLightIntensity[0]);
	
	// Setup the directional light
	vec3 directionalLightDirection = vec3(4.0f, 1.0f, 9.0f);
	normalize(directionalLightDirection);
	glUniform3fv(gDirectionalLightDirectionLoc, 1, &directionalLightDirection[0]);
	vec3 directionalLightIntensity = vec3(0.7f, 0.7f, 0.7f);
	glUniform3fv(gDirectionalLightIntensityLoc, 1, &directionalLightIntensity[0]);

}

static void addShader(GLuint shaderProgram, const char* pShaderText, GLenum shaderType)
{
    GLuint shaderObj = glCreateShader(shaderType);

    if (shaderObj == 0) 
	{
        cerr<<"Error creating shader type "<<shaderType<<endl;
        exit(0);
    }

    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0]= strlen(pShaderText);
    glShaderSource(shaderObj, 1, p, Lengths);
    glCompileShader(shaderObj);
    GLint success;
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
    if (!success) 
	{
        GLchar InfoLog[1024];
        glGetShaderInfoLog(shaderObj, 1024, NULL, InfoLog);
        cerr<<"Error compiling shader type "<<shaderType<<": "<<InfoLog<<endl;
        exit(1);
    }

    glAttachShader(shaderProgram, shaderObj);
}

const string readFileToString(const char* filename)
{
	ifstream file (filename, ios::in);
	if (file.is_open())
	{
        stringstream continut;
        continut << file.rdbuf();
        continut << '\0';
		return continut.str();
	}
    return "";
}

static void buildShaders()
{
    GLuint shaderProgram = glCreateProgram();

    if (shaderProgram == 0) 
	{
        cerr<<"Error creating shader program\n";
        exit(1);
    }

	string VS = readFileToString("vertexShader.glsl");
	string FS = readFileToString("fragmentShader.glsl");

    addShader(shaderProgram, VS.c_str(), GL_VERTEX_SHADER);
    addShader(shaderProgram, FS.c_str(), GL_FRAGMENT_SHADER);

    GLint success = 0;
    GLchar errorLog[1024] = { 0 };

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (success == 0) 
	{
		glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
		cerr<<"Error linking shader program: "<<errorLog<<endl;
        exit(1);
	}

    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
    if (!success) 
	{
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        cerr<<"Error linking shader program: "<<errorLog<<endl;
        exit(1);
    }

    glUseProgram(shaderProgram);

    gModelToWorldTransformLoc = glGetUniformLocation(shaderProgram, "gModelToWorldTransform");
    assert(gModelToWorldTransformLoc != 0xFFFFFFFF);
    gWorldToViewToProjectionTransformLoc = glGetUniformLocation(shaderProgram, "gWorldToViewToProjectionTransform");
    assert(gWorldToViewToProjectionTransformLoc != 0xFFFFFFFF);
	gAmbientLightIntensityLoc = glGetUniformLocation(shaderProgram, "gAmbientLightIntensity");
	assert(gAmbientLightIntensityLoc != 0xFFFFFFFF);
	gDirectionalLightIntensityLoc = glGetUniformLocation(shaderProgram, "gDirectionalLightIntensity");
	assert(gDirectionalLightIntensityLoc != 0xFFFFFFFF);
	gDirectionalLightDirectionLoc = glGetUniformLocation(shaderProgram, "gDirectionalLightDirection");
	assert(gDirectionalLightDirectionLoc != 0xFFFFFFFF);
    gKaLoc = glGetUniformLocation(shaderProgram, "gKa");
	assert(gDirectionalLightDirectionLoc != 0xFFFFFFFF);
    gKdLoc = glGetUniformLocation(shaderProgram, "gKd");
	assert(gDirectionalLightDirectionLoc != 0xFFFFFFFF);

	gColourLoc = glGetUniformLocation(shaderProgram, "gColour");
	assert(gDirectionalLightDirectionLoc != 0xFFFFFFFF);


}



int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("obj Loader Test");

    initializeGlutCallbacks();

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) 
    {
		cerr<<"Error: "<<glewGetErrorString(res)<<"\n";
		return 1;
    }

	buildShaders();

	// Enable the z-buffer
	glEnable(GL_DEPTH_TEST);
	// GL_LESS - Passes if the incoming depth value is less than the stored depth value
	glDepthFunc(GL_LESS);

	//Enable backface culling
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW); 

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 

	// Initialise lights
	initLights();
	 
	// Create a vertex buffer
	createVertexBuffer();

	glutKeyboardFunc(myKeyboard);

    glutMainLoop();
    
    return 0;
}