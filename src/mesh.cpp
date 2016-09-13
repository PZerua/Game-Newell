#include "mesh.h"
#include <cassert>
#include "shader.h"

Mesh::Mesh()
{
	vertices_vbo_id = 0;
	uvs_vbo_id = 0;
}

Mesh::Mesh(const Mesh& m)
{
	vertices = m.vertices;
	uvs = m.uvs;
}

Mesh::~Mesh()
{
	if (vertices_vbo_id) glDeleteBuffersARB(1, &vertices_vbo_id);
	if (uvs_vbo_id) glDeleteBuffersARB(1, &uvs_vbo_id);

	clear();
}

void Mesh::clear()
{
	vertices.clear();
	uvs.clear();
}

void Mesh::render(int primitive, Shader* sh)
{
	assert(vertices.size() && "No vertices in this mesh");

	int vertex_location = sh->getAttribLocation("a_vertex");
	assert(vertex_location != -1 && "No a_vertex found in shader");

	if (vertex_location == -1)
		return;

	glEnableVertexAttribArray(vertex_location);
	if (vertices_vbo_id)
	{
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertices_vbo_id);
		glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}
	else
		glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, &vertices[0]);

	int uv_location = -1;
	if (uvs.size())
	{
		uv_location = sh->getAttribLocation("a_uv");
		if (uv_location != -1)
		{
			glEnableVertexAttribArray(uv_location);
			if (uvs_vbo_id)
			{
				glBindBufferARB(GL_ARRAY_BUFFER_ARB, uvs_vbo_id);
				glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, 0, NULL);
			}
			else
				glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, 0, &uvs[0]);
		}
	}

	glDrawArrays(primitive, 0, (GLsizei)vertices.size());

	glDisableVertexAttribArray(vertex_location);
	if (uv_location != -1) glDisableVertexAttribArray(uv_location);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

void Mesh::uploadToVRAM()
{
	//delete old
	if (vertices_vbo_id) glDeleteBuffersARB(1, &vertices_vbo_id);
	if (uvs_vbo_id) glDeleteBuffersARB(1, &uvs_vbo_id);

	glGenBuffersARB(1, &vertices_vbo_id); //generate one handler (id)
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertices_vbo_id); //bind the handler
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertices.size() * 3 * sizeof(float), &vertices[0], GL_STATIC_DRAW_ARB); //upload data

	if (uvs.size())
	{
		glGenBuffersARB(1, &uvs_vbo_id); //generate one handler (id)
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, uvs_vbo_id); //bind the handler
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, uvs.size() * 2 * sizeof(float), &uvs[0], GL_STATIC_DRAW_ARB); //upload data
	}
}

void Mesh::createQuadUVs(float center_x, float center_y, float w, float h, int row, int col)
{
	vertices.clear();
	uvs.clear();

	//create six vertices (3 for upperleft triangle and 3 for lowerright)

	vertices.push_back(Vector3(center_x + w*0.5f, center_y + h*0.5f, 0.0f));
	vertices.push_back(Vector3(center_x - w*0.5f, center_y - h*0.5f, 0.0f));
	vertices.push_back(Vector3(center_x + w*0.5f, center_y - h*0.5f, 0.0f));
	vertices.push_back(Vector3(center_x - w*0.5f, center_y + h*0.5f, 0.0f));
	vertices.push_back(Vector3(center_x - w*0.5f, center_y - h*0.5f, 0.0f));
	vertices.push_back(Vector3(center_x + w*0.5f, center_y + h*0.5f, 0.0f));

	//texture coordinates
	uvs.push_back(Vector2(0.125f + 0.125f * col, 0.0f + 0.125f * row));
	uvs.push_back(Vector2(0.0f + 0.125f * col, 0.125f + 0.125f * row));
	uvs.push_back(Vector2(0.125f + 0.125f * col, 0.125f + 0.125f * row));
	uvs.push_back(Vector2(0.0f + 0.125f * col, 0.0f + 0.125f * row));
	uvs.push_back(Vector2(0.0f + 0.125f * col, 0.125f + 0.125f * row));
	uvs.push_back(Vector2(0.125f + 0.125f * col, 0.0f + 0.125f * row));
}

void Mesh::createQuad(float center_x, float center_y, float w, float h)
{
	vertices.clear();

	//create six vertices (3 for upperleft triangle and 3 for lowerright)

	vertices.push_back(Vector3(center_x + w*0.5f, center_y + h*0.5f, 0.0f));
	vertices.push_back(Vector3(center_x - w*0.5f, center_y - h*0.5f, 0.0f));
	vertices.push_back(Vector3(center_x + w*0.5f, center_y - h*0.5f, 0.0f));
	vertices.push_back(Vector3(center_x - w*0.5f, center_y + h*0.5f, 0.0f));
	vertices.push_back(Vector3(center_x - w*0.5f, center_y - h*0.5f, 0.0f));
	vertices.push_back(Vector3(center_x + w*0.5f, center_y + h*0.5f, 0.0f));
}

void Mesh::setUVs(int row, int col)
{
	uvs.clear();

	uvs.push_back(Vector2(0.125f + 0.125f * col, 0.0f + 0.125f * row));
	uvs.push_back(Vector2(0.0f + 0.125f * col, 0.125f + 0.125f * row));
	uvs.push_back(Vector2(0.125f + 0.125f * col, 0.125f + 0.125f * row));
	uvs.push_back(Vector2(0.0f + 0.125f * col, 0.0f + 0.125f * row));
	uvs.push_back(Vector2(0.0f + 0.125f * col, 0.125f + 0.125f * row));
	uvs.push_back(Vector2(0.125f + 0.125f * col, 0.0f + 0.125f * row));
}
