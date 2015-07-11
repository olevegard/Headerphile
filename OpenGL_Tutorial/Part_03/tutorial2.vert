#version 330
// in_Position was bound to attribute index 0 and in_Color was bound to attribute index 1
in vec4 in_Color;
in vec3 in_Position;
 
// We output the ex_Color variable to the next shader in the chain
out vec4 color;

void main(void) {
	// Set the position to the one defined in our vertex array
    gl_Position = vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
 
    // Pass the color on to the fragment shader
    //ex_Color = in_Color;
	color = in_Color;
}

