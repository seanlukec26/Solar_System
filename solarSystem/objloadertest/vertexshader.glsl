#version 330
 
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
 
 
// Transforms
uniform mat4 gModelToWorldTransform;
uniform mat4 gWorldToViewToProjectionTransform;
 
out vec4 Color0;
out vec3 Normal0;
                               
void main()
{  
    // Transform the vertex from local space to homogeneous clip space
    vec4 vertexPositionInModelSpace = vec4(Position, 1);
    vec4 vertexInWorldSpace = gModelToWorldTransform * vertexPositionInModelSpace;
    vec4 vertexInHomogeneousClipSpace = gWorldToViewToProjectionTransform * vertexInWorldSpace;
    gl_Position = vertexInHomogeneousClipSpace;
 
 
    // Calculate the directional light intensity at the vertex
    // Find the normal in world space and normalise it
    vec3 normalInWorldSpace = (gModelToWorldTransform * vec4(Normal, 0.0)).xyz;
    normalInWorldSpace = normalize(normalInWorldSpace);
    Normal0 = normalInWorldSpace;
       
    // Final vertex colour is the product of the vertex colour
    // and the total light intensity at the vertex
    Color0 = vec4(1.0, 1.0, 1.0, 1.0);
 
}

/*#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;

out vec4 LightIntensity0;

// Transforms
uniform mat4 gModelToWorldTransform;
uniform mat4 gWorldToViewToProjectionTransform;

// Ambient light parameters
uniform vec3 gAmbientLightIntensity;

// Directional light parameters
uniform vec3 gDirectionalLightIntensity;
uniform vec3 gDirectionalLightDirection;

// Material constants
uniform float gKa;
uniform float gKd;

                                                                   
void main()
{  
	// Transform the vertex from local space to homogeneous clip space 
	vec4 vertexPositionInModelSpace = vec4(Position, 1);
	vec4 vertexInWorldSpace = gModelToWorldTransform * vertexPositionInModelSpace;
	vec4 vertexInHomogeneousClipSpace = gWorldToViewToProjectionTransform * vertexInWorldSpace;
	gl_Position = vertexInHomogeneousClipSpace;

	// Calculate the ambient light intensity at the vertex
	// Ia = Ka * ambientLightIntensity
	vec4 ambientLightIntensity = gKa * vec4(gAmbientLightIntensity, 1.0);

	// Calculate the directional light intensity at the vertex
	// Find the normal in world space and normalise it
	vec3 normalInWorldSpace = (gModelToWorldTransform * vec4(Normal, 0.0)).xyz;
	normalInWorldSpace = normalize(normalInWorldSpace);
	// Setup the light direction and normalise it
	vec3 lightDirection = normalize(-gDirectionalLightDirection);


	//lightDirection = normalize(gDirectionalLightDirection);
	// Id = kd * lightItensity * N.L
	// Calculate N.L
	float diffuseFactor = dot(normalInWorldSpace, lightDirection);
	diffuseFactor = clamp(diffuseFactor, 0.0, 1.0);
	// N.L * light source colour * intensity
    vec4 diffuseLightIntensity = gKd * vec4(gDirectionalLightIntensity, 1.0f) * diffuseFactor;
   
	// Final vertex colour is the product of the vertex colour
	// and the total light intensity at the vertex 
	LightIntensity0 = ambientLightIntensity + diffuseLightIntensity;

}*/

