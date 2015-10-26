#version 330
// It was expressed that some drivers required this next line to function properly
precision highp float;
 
in vec4 ex_Color;
in vec3 ex_Pos;
in vec4 ex_Light;

out vec4 fragColor;
 
void main(void) {
	/*
	vec3 dist( ex_Light.x - ex_Pos.x, ex_Light.y - ex_Pos.y, ex_Light.z - ex_Pos.z );
	float d = sqrt( ( dist.x * dist.x) + ( dist.y * dist.y) + ( dist.y * dist.y))

	fragColor.x += d;
	fragColor.y += d;
	fragColor.z += d;
	// fragColor = vec4(ex_Pos, 1.0f);
	*/

	vec3 dir(0.0f, 1.0, 0.0);
	vec3 clr(1.0, 1.0, 0.0);
	vec3 lclr(1.0, 0.0, 1.0);

	fragColor = ex_Color;
	float diffuseIntensity = max(0.0, dot(normalize(exColor), -dir)); 
	fragColor = clr * vec4(lclr*(1.0+fDiffuseIntensity), 1.0); 
}
