#version 330
// It was expressed that some drivers required this next line to function properly
precision highp float;
 
in vec4 ex_Color;

out vec4 fragColor;
 
void main(void) {
	fragColor = ex_Color;
}
