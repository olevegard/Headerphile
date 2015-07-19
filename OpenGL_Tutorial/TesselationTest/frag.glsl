#version 330
// It was expressed that some drivers required this next line to function properly
precision highp float;
 
in vec4 ex_Color;

out vec4 fragColor;
 
float rand(vec2 co);

bool randomColor = false;

void main(void) {
	if (randomColor)
	{
		fragColor.r  = (ex_Color.r * 0.5) + ( rand(ex_Color.ra) * 0.5);
		fragColor.g  = (ex_Color.g * 0.5) + ( rand(ex_Color.bg) * 0.5);
		fragColor.b  = (ex_Color.b * 0.5) + ( rand(ex_Color.gb) * 0.5);
		fragColor.a  = ex_Color.a;// + ( rand(ex_Color.ba) * 0.8);
	} else
		fragColor = ex_Color;
}

/*
float rand(vec2 co)
{
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
*/
float rand(vec2 co)
{
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt= dot(co.xy ,vec2(a,b));
    highp float sn= mod(dt,3.14);
    return fract(sin(sn) * c);
}
