#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 100) out;

in vec4 color[]; // Output from vertex shader for each vertex

out vec4 ex_Color; // Output to fragment shader

void MakeEar(vec4 v);
void MakeBasicTriangle();
void MakeSplitTriangle(float dist);
void MakeTriangle(vec4 v1, vec4 v2, vec4 v3);

vec4 corner1 = gl_in[0].gl_Position;
vec4 corner2 = gl_in[1].gl_Position;
vec4 center = gl_in[2].gl_Position;

// How far the pieces of the triangles will be from eachother
float explodeDist = 0.5;

bool makeNormal = true;
bool makeExploded = true;
bool makeEars = true;

void main()
{
	if (makeExploded)
		// Make the "exploded" triangle
		MakeSplitTriangle(explodeDist);

	if (makeNormal)
		// MakeBasicTriangle();
		MakeBasicTriangle();

	if (makeEars)
	{
		// Make triangles on the outer corners of each triangle
		MakeEar(corner1);
		MakeEar(corner2);
	}
}

void MakeSplitTriangle(float dist)
{
	// Find middle of the two outer corners of the triangle
	vec4 diff = ( corner1 - corner2 ) * dist;

	// Generate an offset vector
	vec4 offset = vec4(0.0f);

	// Flip in order to get a normal ( a vector perpendicular to diff )
	// This is the direction the triangles will move
	offset.x = diff.y;
	offset.y = diff.x;

	// Set new positions
	corner1 += offset;
	corner2 += offset;
	center += offset;

	MakeTriangle( center, corner1, corner2);
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

// Draws a symetric triangle so that it looks like the square has ears
void MakeEar(vec4 v)
{
	vec4 v1 = v + vec4(0.0, 1.0 * v.y, 0.0, 0.0);

	vec4 v2 = v + vec4(1.0 * v.x, 0.0, 0.0, 0.0);

	vec4 v3 = v + vec4(0.0, 0.0, 0.0, 0.0);

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
