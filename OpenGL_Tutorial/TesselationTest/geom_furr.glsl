#version 330

layout(lines) in;
layout(line_strip, max_vertices = 10) out;

in vec4 color[]; // Output from vertex shader for each vertex

out vec4 ex_Color; // Output to fragment shader

void MakeBasicTriangle();
void MakeTriangle(vec4 v1, vec4 v2);
void MakeFurr(vec4 v1);

void main()
{
	MakeBasicTriangle();

	vec4 v1 = gl_in[0].gl_Position;
	vec4 v2 = gl_in[1].gl_Position;

	while (v1.x < v2.x)
	{
		MakeFurr(v1);
		v1.x += 0.02;
	}
}

void MakeBasicTriangle()
{
	vec4 v1 = gl_in[0].gl_Position;
	vec4 v2 = gl_in[1].gl_Position;

		MakeTriangle( v1, v2 );
}

void MakeFurr(vec4 v1)
{
	vec4 v2 = v1;
	v2.y = v1.y + 0.1;

	MakeTriangle(v1, v2);
}

// Makes a simple triangle witht the colors passed in from the previous shaders
void MakeTriangle(vec4 v1, vec4 v2)
{
	// Set position of new vertex
	gl_Position = v1;

	// Set color of new vertex
	ex_Color = color[0];

	// Create new vertex
	// Think of gl_Position and ex_Color as a temporary variable
	// that we'll re-use for every vertex we make
	EmitVertex();

	gl_Position = v2;
	ex_Color = color[1];
	EmitVertex();

	// Tell OpenGL we're done with this triangle
	// After this, the three calls to EmitVertex() will make a triangle
	// Now the next call to EmitVertex() will make a vertex for a new triangle
	EndPrimitive();
}

