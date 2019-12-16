#include "pch.h"
#include "MeshRenderer.h"
#include "Entity.h"
#include "Common.h"
#include "Application.h"
#include "Camera.h"

static GLfloat iX = 0;
static GLfloat iY = 0;
static GLfloat iZ = 0;

MeshRenderer::MeshRenderer(std::shared_ptr <Model> model, std::shared_ptr <ShaderProgram> program, std::shared_ptr<Texture> texture)
{
	m_model = model;
	m_program = program;
	m_texture = texture;
}

void MeshRenderer::OnUpdate(float deltaTime)
{
	//LOG_DEBUG("I'm Updating!");
}

void MeshRenderer::OnRender()
{
	m_program->Use();

	//Set only uniforms that are common to every shader program here
	m_program->setMat4("model", m_entity->GetTransform()->GetTransformationMatrix());

	m_texture->Bind();

	for (Mesh* mesh : m_model->GetMeshes())
	{
		mesh->Bind();

		if (m_texture) m_texture->Bind();

		GL_ATTEMPT(glDrawElements(GL_TRIANGLES, mesh->GetIndiciesCount(), GL_UNSIGNED_INT, 0));
	}

}

void MeshRenderer::OnAttach()
{

}
