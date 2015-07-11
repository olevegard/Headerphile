#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 100) out;

in vec4 color[]; // Output from vertex shader for each vertex

out vec4 ex_Color; // Output to fragment shader


void MakeEar(vec4 v);
void SplitVertex(vec4 v, vec4 v2, float factor);
void MakeBasicTriangle();

void main()
{
	//MakeBasicTriangle();

	MakeEar(gl_in[1].gl_Position);
	MakeEar(gl_in[0].gl_Position);

	SplitVertex(gl_in[0].gl_Position, gl_in[1].gl_Position, 0.3333f );
	SplitVertex(gl_in[1].gl_Position, gl_in[2].gl_Position, 0.3333f );

	// MakeBasicTriangle();

}

// This is the basic geometry shader.
// It will just draw the base triangles from our vertexes
// If we just call this, it would be like we had used the standard OpenGL shader
void MakeBasicTriangle()
{
	gl_Position = gl_in[0].gl_Position;
	ex_Color = vec4(1.0f, 0.0f, 0.0f, 0.5f);
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	ex_Color = vec4(1.0f, 0.0f, 0.0f, 0.5f);
	EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	ex_Color = vec4(1.0f, 0.0f, 0.0f, 0.5f);
	EmitVertex();

    EndPrimitive();
}

void SplitVertex(vec4 v, vec4 v2, float factor)
{

	// Make angled lines
	vec4 distVec = v - v2;
	distVec.a = 1.0f;
	float x = -1.0 * ( ( 1.0f - factor ) * v.x);
	float y = -1.0 * ( ( 1.0f - factor ) * v.y);

	gl_Position = v + vec4(0.0, y, 0.0, 0.0);
	// ex_Color = distVec;
	ex_Color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	EmitVertex();

	gl_Position = v + vec4(x, y, 0.0, 0.0);
	ex_Color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	EmitVertex();

	gl_Position = v + vec4(x, 0.0, 0.0, 0.0);
	ex_Color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	EmitVertex();

    EndPrimitive();

	float x2 = -1.0 * ( ( 1.0f - factor ) * v2.x);
	float y2 = -1.0 * ( ( 1.0f - factor ) * v2.y);

	gl_Position = v2 + vec4(0.0, y2, 0.0, 0.0);
	ex_Color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	EmitVertex();

	gl_Position = v2 + vec4(x2, y2, 0.0, 0.0);
	ex_Color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	EmitVertex();

	gl_Position = v2 + vec4(x2, 0.0, 0.0, 0.0);
	ex_Color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	EmitVertex();

    EndPrimitive();

	// Make straight lines

	gl_Position = v + vec4(x, y, 0.0, 0.0);
	ex_Color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	EmitVertex();

	gl_Position = v2 + vec4(x2, y2, 0.0, 0.0);
	ex_Color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	EmitVertex();

	gl_Position = v2 + vec4(distVec.x * x2, distVec.y * y2, 0.0, 0.0);
	// gl_Position = v2 + distVec;

	ex_Color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	EmitVertex();

    EndPrimitive();

	//gl_Position = v2 + vec4(0.0f, y2, 0.0, 0.0);
	gl_Position = v2 + vec4(distVec.x * x2, distVec.y * y2, 0.0, 0.0);
	ex_Color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	EmitVertex();

	gl_Position = v + vec4(distVec.x * x, distVec.y * y, 0.0, 0.0);
	// gl_Position = v + vec4(0.0f, y, 0.0, 0.0);
	ex_Color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	EmitVertex();

	gl_Position = v + vec4(x, y, 0.0, 0.0);
	ex_Color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	EmitVertex();

    EndPrimitive();

/*
	gl_Position = v + vec4(0.0, -1.0 * factor * v.y, 0.0, 0.0);
	EmitVertex();

	gl_Position = v + vec4(-1.0 * factor * v.x, -1.0 * factor *  v.y, 0.0, 0.0);
	EmitVertex();

	gl_Position = v + vec4(-1.0 * factor * v.x, 0.0, 0.0, 0.0);
	EmitVertex();

    EndPrimitive();
	*/
}

// Draws a symetric triangle so that it looks like the square has ears
void MakeEar(vec4 v)
{
	gl_Position = v + vec4(0.0, 1.0 * v.y, 0.0, 0.0);
	ex_Color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	EmitVertex();

	gl_Position = v + vec4(1.0 * v.x, 0.0, 0.0, 0.0);
	ex_Color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	EmitVertex();

	gl_Position = v + vec4(0.0, 0.0, 0.0, 0.0);
	ex_Color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	EmitVertex();

    EndPrimitive();
}



