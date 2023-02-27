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
//	vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));
//	frag_color = vec4(textureColor,1);

	/* Lights */
	
	vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));
	/* Ambient Reflection - Ia = La * Ka */
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

	// frag_color=vec4(Is+Id+Ia,1);
	color = vec4(Is + Id + Ia, 1);

	/* Mapping */
	float level=floor(pow((max(0.0f, (dot(light.location-fragPos, normalize(fragNormal))))), light.alfa) * levels);
	float brightness=level/levels;


	if(Normal)
	{
		vec3 normal = vec3(texture(material.textureMap, fragTexCoords)).rgb;
		normal = normalize(normal * 2.0 - 1.0);   
		vec3 I_A = (textureColor * light.La) ;
		float temp_cosQ = max(dot(normalize(normal), light.location - fragPos), 0.0);
		vec3 I_D = material.Kd * temp_cosQ * light.Ld;
		float temp_q = max(dot(r, v), 0.0);
		vec3 I_S =  light.Ls * pow(temp_q, light.alfa) * material.Ks;
		vec3 final_light = (I_A + I_D + I_S);
		frag_color = vec4(final_light, 1);
	}
//	else if(Environment)
//	{
//		vec4 reflected_color = texture(enviro_map, reflected);
//		frag_color = color * (1 - 0.6) + reflected_color * 0.6;
//	}
//	else if(Toonshading)
//	{
//		Is = light.Ls * brightness * material.Ks;
//		frag_color = vec4(brightness * (Ia + Id + Is), 1);
//	}
	else
	{
		frag_color = vec4(Is + Id + Ia, 1);
	}
	
}
