#include "Component/SpriteComponent.hpp"

#include "Actor/Actor.hpp"
#include "Manager.hpp"
#include "Renderer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Definitions.hpp"

SpriteComponent::SpriteComponent(Actor* owner, Texture* tex, int order)
    :Component(owner)
    // ,mTextureUnit(GL_TEXTURE0)
    ,m_Texture(tex)
    ,m_ShaderDesc({
        "SpriteShader",
        {
        std::string(SHADER_PATH) + "sprite.vert",
        std::string(SHADER_PATH) + "sprite.frag"
        },
        Shader::ShaderDesc::VERTEX_FRAGMENT
    })
    ,m_DisableDrawing(false)
{
   	std::vector<glm::vec3> vertices = {
		glm::vec3(-0.5f, 0.5f, 0.f), // top left
		glm::vec3(0.5f, 0.5f, 0.f),  // top right
		glm::vec3(0.5f,-0.5f, 0.f),  // bottom right
		glm::vec3(-0.5f,-0.5f, 0.f)  //bottom left
	};
   	std::vector<glm::vec2> texcoords = {
		glm::vec2(0.f, 1.f),  // bottom left
		glm::vec2(1.f, 1.f), // bottom right
        glm::vec2(1.f, 0.f), // top right
        glm::vec2(0.f, 0.f) // top left
	};
	std::vector<unsigned int> indices = {
		0, 1, 2,
		2, 3, 0
	};
    mVAO = new VertexArray(vertices, texcoords, indices);
    owner->GetManager()->m_Renderer->AddSpriteComp(this, order);
}

SpriteComponent::~SpriteComponent()
{
    delete mVAO;
}

void SpriteComponent::Draw()
{
    if (m_DisableDrawing) {
        return;
    }
    
	float screenWidth = m_Owner->GetManager()->GetScreenSize().x;
	float screenHeight = m_Owner->GetManager()->GetScreenSize().y;
	glm::mat4 spriteViewProj = glm::mat4({
        { 2.0f/screenWidth, 0.0f, 0.0f, 0.0f },
        { 0.0f, 2.0f/screenHeight, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 1.0f }
    });

    Shader* shader = m_Owner->GetManager()->m_Renderer->GetShader(m_ShaderDesc);
    shader->UseProgram();

    shader->SetMatrixUniform("ModelTransform", m_Owner->GetWorldTransform());
    shader->SetMatrixUniform("SpriteViewProj", spriteViewProj);

    // VertexArray* vao = mMesh->GetVertexArray();
    mVAO->SetActive();
    m_Texture->BindTexture();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    m_Texture->UnBindTexture();
}

