#version 330
// in_Position was bound to attribute index 0 and in_Color was bound to attribute index 1
in vec3 in_Position;
in vec4 in_Color;
in vec3 in_Normals;

uniform mat4 mvp;
uniform mat4 normal;
uniform vec4 light;
 
// We output the ex_Color variable to the next shader in the chain
out vec4 ex_Color;
// out vec3 ex_Pos;
// out vec4 ex_Light;

void main(void) {
	// Set the position to the one defined in our vertex array
	vec4 pos = vec4(in_Position, 1.0f);
	vec4 col = vec4(in_Normals, 1.0f);

	gl_Position = mvp * pos;

	//ex_Pos = in_Position;
	float cosTheta = clamp( dot(normalize(pos), normalize(light)), 0, 1);

    // Pass the color on to the fragment shader
	// ex_Color = in_Color;
	ex_Color = normalize(normal * col);
	// ex_Color *= cosTheta;
	ex_Color.w = 1.0f;
}
