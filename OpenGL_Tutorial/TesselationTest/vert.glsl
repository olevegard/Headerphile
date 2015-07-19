#version 330
// in_Position was bound to attribute index 0 and in_Color was bound to attribute index 1
// in vec4 in_Color;
in vec3 in_Position;

uniform mat4 mvp;
 
// We output the ex_Color variable to the next shader in the chain
out vec4 color;
out vec3 vPosition;

void main(void) {
	// Set the position to the one defined in our vertex array
	vec4 pos = vec4(in_Position, 1.0f);

	gl_Position = mvp * pos;
	vPosition = (mvp * pos).xyz;
	// gl_Position = pos;

    // Pass the color on to the fragment shader
	//color = in_Color;
	color = vec4( 1.0f, 0.0f, 0.0f, 1.0f );
}
