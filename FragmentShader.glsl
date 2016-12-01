#version 330 core

in vec2 UV;
in vec3 vertexPositionWorld;
in vec3 normalWorld;

out vec4 daColor;

uniform vec3 ambient;
uniform vec3 lightPositionWorld; 
uniform vec3 eyePositionWorld; 

uniform sampler2D myTextureSampler;
uniform sampler2D myTextureSampler2;

uniform vec3 diffuseBrightness;
uniform float SpecularBrightness;

void main()
{
	//object texture
	vec3 color = texture( myTextureSampler, UV ).rgb; 
	vec3 color2 = texture( myTextureSampler2, UV ).rgb;
	//color *= theColor; //to integrate texture and vertex color
	vec3 objectColor = mix(color, color2, 0.5);    
    
	// Ambient
	vec3 ambientLight = objectColor * ambient;
    // Diffuse 
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = dot(lightVectorWorld, normalize(normalWorld));
	vec3 diffuseLight = vec3(brightness, brightness, brightness);
	// Specular
	vec3 reflectedLightVectoreWorld = reflect(-lightVectorWorld, normalWorld);
	vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
	float s = clamp(dot(reflectedLightVectoreWorld, eyeVectorWorld), 0, 1);
	s = pow(s ,50);
	vec3 speculatLight = vec3(s, 0, 0);
	
	vec3 result = (ambientLight + clamp(diffuseLight, 0, 1) * diffuseBrightness + speculatLight) * objectColor;
	daColor = vec4(result, 1.0f);
	
}	