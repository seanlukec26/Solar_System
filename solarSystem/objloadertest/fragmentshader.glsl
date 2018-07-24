#version 330
 
in vec3 Normal0;
in vec4 Color0;
 
out vec4 FragColor;
 
uniform sampler2D gTextureSampler;
 
// Material constants
uniform float gKa;
uniform float gKd;

uniform vec4 gColour;
 
 
// Ambient light parameters
uniform vec3 gAmbientLightIntensity;
 
// Directional light parameters
uniform vec3 gDirectionalLightIntensity;
uniform vec3 gDirectionalLightDirection;

 
void main()
{
    // Setup the light direction and normalise it
    vec3 lightDirection = normalize(-gDirectionalLightDirection);
 
    float diffuseFactor = dot(Normal0, lightDirection);
    diffuseFactor = clamp(diffuseFactor, 0.0, 1.0);
 
    vec4 diffuseLightIntensity = gKd * vec4(gDirectionalLightIntensity, 1.0f) * diffuseFactor;
 
    // Calculate the ambient light intensity at the vertex
    vec4 ambientLightIntensity = gKa * vec4(gAmbientLightIntensity, 1.0);


	// calculate Specular lighting
	vec4 specularLightIntensity = vec4(1.0f,1.0f,1.0f,1.0f) * pow(max(dot(normalize(-reflect(lightDirection,Normal0)),lightDirection),0.0),0.05*45.0f);
	specularLightIntensity = clamp(specularLightIntensity, 0.0, 1.0);
 
	//FragColor = Colour0 * (ambientLightIntensity + diffuseLightIntensity);
    FragColor = gColour * (ambientLightIntensity + diffuseLightIntensity + specularLightIntensity);
 
}
