#version 330 core

struct Material
{
	sampler2D textureMap;
	// You can add more fields here...
	// Such as:
	//		1. diffuse/specular relections constants
	//		2. specular alpha constant
	//		3. Anything else you find relevant
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

// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 orig_fragPos;
// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{
    vec3 Ia; /*ambient*/ 
	vec3 Id; /*diffuse*/
	vec3 Is; /*specular*/
	// Sample the texture-map at the UV coordinates given by 'fragTexCoords'
	vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));
	/* Ambient Reflection - Ia=La*Ka */
	/* La-ambient light intensity, Ka-ambient color*/
	Ia = light.La * material.Ka;

	/* Diffuse Reflection - Id=Kd(l*n)Ld */
	/* Ld-Source diffuse intensity, Kd-Surface diffuse reflection coefficient ,(l*n)=cos teta */
	vec3 l = normalize(light.location-fragPos);
    Id = material.Kd * max(0.0f, (dot(light.location-fragPos, normalize(fragNormal)))) * light.Ld;

	/* Specular Reflection - Is=Ks(r*v)^alfa*Ls */
	/*Ks-Surface specular reflection coefficient, (r*v)=cos teta, alfa-Shininess coefficient, Ls-Source specular intensity */
	vec3 r = l - 2 * dot(l, normalize(fragNormal)) * normalize(fragNormal);
	vec3 v = normalize(fragPos - camera_eyes);
    Is =material.Ks * pow(max(dot(r, v), 0.0f), (light.alfa)) * light.Ls;
	// frag_color = vec4(1.0, 0.0, 0.0, 1.0); // Red
	frag_color=vec4(Is+Id+Ia,1);
}
