#version 330 core

// interpolated values from the vertex shaders
in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

// light properties
struct Light
{
	vec3 pos;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
	vec3 att;	// constant, linear, quadratic
};

// uniform input data
uniform vec3 uViewpoint;
uniform Light uLight;
uniform sampler2D uDiffuseMap;
uniform sampler2D uSpecularMap;
uniform float uShininess;

// output data
out vec3 fColor;

void main()
{
	// fragment normal
    vec3 n = normalize(vNormal);

	// vector toward the viewer
	vec3 v = normalize(uViewpoint - vPosition);

	// vector towards the light
    vec3 l = normalize(uLight.pos - vPosition);

	// halfway vector
	vec3 h = normalize(l + v);

	// calculate ambient, diffuse and specular intensities
	vec3 Ia = uLight.La * texture(uDiffuseMap, vTexCoord).rgb;
	vec3 Id = vec3(0.0f);
	vec3 Is = vec3(0.0f);
	float dotLN = max(dot(l, n), 0.0f);

	if(dotLN > 0.0f)
	{
		// attenuation
		float dist = length(uLight.pos - vPosition);
		float attenuation = 1.0f / (uLight.att.x + dist * uLight.att.y + dist * dist * uLight.att.z);

		Id = uLight.Ld * texture(uDiffuseMap, vTexCoord).rgb * dotLN * attenuation;
		Is = uLight.Ls * texture(uSpecularMap, vTexCoord).rgb * pow(max(dot(n, h), 0.0f), uShininess) * attenuation;
	}
	
	// intensity of reflected light
	fColor = Ia + Id + Is;
}
