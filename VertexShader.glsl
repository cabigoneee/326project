#version 330 core

in layout(location = 0) vec3 vertexPosition_modelspace;
in layout(location = 1) vec2 vertexUV;
in layout(location = 2) vec3 normal;


out vec2 UV;
out vec3 normalWorld;
out vec3 vertexPositionWorld;

uniform mat4 projectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 rotationMatrix;

void main(){

    gl_Position = projectionMatrix * ViewMatrix * rotationMatrix * vec4(vertexPosition_modelspace, 1.0f);
	vec4 newPosition = (ViewMatrix * rotationMatrix) * vec4(vertexPosition_modelspace, 1.0f);
	
	vec4 normal_temp = ViewMatrix * rotationMatrix * vec4(normal, 0);
	normalWorld = normal_temp.xyz;
	vertexPositionWorld = newPosition.xyz;

    UV = vertexUV;
	
	
}

