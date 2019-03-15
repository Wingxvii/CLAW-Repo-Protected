#version 420

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform float healthAmount;

layout(location = 0) in vec3 in_vert;    
layout(location = 1) in vec2 in_uv;      
layout(location = 2) in vec3 in_normal;  
layout(location = 3) in vec3 in_vert2;    
layout(location = 4) in vec2 in_uv2;      
layout(location = 5) in vec3 in_normal2;  

out vec3 norm;
out vec3 pos;
out float healthValue;

void main()
{
	healthValue = healthAmount;
    vec3 interp_vert = in_vert * (1.0f - healthAmount) + in_vert2 * (healthAmount);
    vec3 interp_normal = in_normal * (1.0f - healthAmount) + in_normal2 * (healthAmount);
    
	norm = mat3(uView) * mat3(uModel) * interp_normal;
	
	vec4 viewSpace = uView * uModel * vec4(interp_vert, 1.0f);
	
	gl_Position = uProj * viewSpace;
	
	pos = viewSpace.xyz;
}