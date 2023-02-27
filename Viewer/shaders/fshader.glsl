#version 330 core

struct Material
{
	sampler2D textureMap;
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
};

struct PointLight
{
    vec3 La;
	vec3 Ld;
	vec3 Ls;
	float alfa;
	vec3 location;
};

// We set this field's properties from the C++ code
uniform Material material;
uniform PointLight light;
uniform vec3 camera_eyes;

uniform bool Normal;
uniform bool Environment;
uniform bool Toonshading;

// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 orig_fragPos;
in vec3 reflected;

// The final color of the fragment (pixel)
out vec4 frag_color;
out vec4 color;

float levels=4 ;//4 different levels of shading 


void main()
{
    vec3 Ia; /*ambient*/ 
	vec3 Id; /*diffuse*/
	vec3 Is; /*specular*/

	/* Texture */
	//	Sample the texture-map at the UV coordinates given by 'fragTexCoords'
	vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));
	frag_color = vec4(textureColor,1);


	
}
