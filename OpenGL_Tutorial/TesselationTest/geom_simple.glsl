#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 100) out;

in vec4 color[]; // Output from vertex shader for each vertex

out vec4 ex_Color; // Output to fragment shader

void MakeBasicTriangle();
void MakeTriangle(vec4 v1, vec4 v2, vec4 v3);

void main()
{
	MakeBasicTriangle();
}

// This is the basic geometry shader.
// It will just draw the base triangles from our vertexes
// If we just call this, it would be like we had used the standard OpenGL shader
void MakeBasicTriangle()
{
	vec4 v1 = gl_in[0].gl_Position;

	vec4 v2 = gl_in[1].gl_Position;

	vec4 v3 = gl_in[2].gl_Position;

	MakeTriangle( v1, v2, v3 );
}

// Makes a simple triangle witht the colors passed in from the previous shaders
void MakeTriangle(vec4 v1, vec4 v2, vec4 v3)
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

	gl_Position = v3;
	ex_Color = color[2];
	EmitVertex();

	// Tell OpenGL we're done with this triangle
	// After this, the three calls to EmitVertex() will make a triangle
	// Now the next call to EmitVertex() will make a vertex for a new triangle
	EndPrimitive();
}
