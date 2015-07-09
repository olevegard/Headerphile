#version 330
// in_Position was bound to attribute index 0 and in_Color was bound to attribute index 1
attribute  vec4 in_Color;
attribute  vec3 in_Position;
 
// We output the ex_Color variable to the next shader in the chain
out vec4 ex_Color;

void main(void) {
    // Since we are using flat lines, our input only had two points: x and y.
    // Set the Z coordinate to 0 and W coordinate to 1
    gl_Position = vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
 
    // Pass the color on to the fragment shader

    ex_Color.x = (in_Color.x + 1.0f) / 2.0f;
    ex_Color.y = (in_Color.y + 1.0f) / 2.0f;
    ex_Color.z = (in_Color.z + 1.0f) / 2.0f;
    ex_Color.a = in_Color.a;

	/*
    ex_Color.y = in_Color.y;
    ex_Color.z = in_Color.z;
    ex_Color.a = in_Color.a;
	*/
}
