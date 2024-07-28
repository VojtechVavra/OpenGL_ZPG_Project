#include "Mesh.hpp"
#include "TextureManager.hpp"

#include <iostream>


Mesh::Mesh( std::vector<std::unique_ptr<Material>>&& material,
			std::vector<std::shared_ptr<struct MeshLoader::MeshEntry>>&& meshEntries, 
			unsigned int elementCount, 
			std::vector<std::shared_ptr<Texture>>&& textures
			//std::unique_ptr<VAO>&& VAO,
			//std::array<std::unique_ptr<VBO>, 4>&& VBO
		  )
		  : m_material(std::move(material)),
			m_meshEntries(std::move(meshEntries)),
			m_elementCount(elementCount),
			m_textures(std::move(textures))
			//m_VAO(VAO),
			//m_VBO(VBO)
{
	// Konstruktor pøesune data do èlenských promìnných pomoci std::move()
	//std::cout << "Mesh created" << std::endl;
}

Mesh::~Mesh()
{
	/*for (int i = 0; i < m_meshEntries.size(); ++i) {
		delete m_meshEntries.at(i);
	}*/
	m_meshEntries.clear();
}


// TODO: podivat se na tuto funkci jeste
void Mesh::render()
{
	/*for (int i = 0; i < m_meshEntries.size(); ++i) {
		// Zajištìní, že všechny objekty jsou správnì inicializovány
		if (!m_meshEntries[i]->vao) {
			std::cout << "Error: VAO is not initialized!" << std::endl;
			return;
		}
		// Kontrola, že všechny VBO jsou inicializovány
		for (const auto& vbo : m_meshEntries[i]->vbo) {
			if (!vbo) {
				std::cout << "Error: VBO is not initialized!" << std::endl;
				return;
			}
		}
	}*/
	
	/*m_VAO->bind();
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // Bind IBO
	m_VBO[VERTEX_BUFFER]->bind();
	m_VBO[TEXCOORD_BUFFER]->bind();
	m_VBO[NORMAL_BUFFER]->bind();
	m_VBO[INDEX_BUFFER]->bind();
	*/

	auto textureManager = TextureManager::getInstance();
	std::vector<std::shared_ptr<Texture>> t;
	for (int j = 0; j < m_material.size(); j++) {
		if (m_material[j]->diffuseMap != "no_texture") {
			t.emplace_back(textureManager->getModelTexture(m_material[j]->diffuseMap));
		}
	}

	int textureNumberToUse = 0;

	for (int i = 0; i < m_meshEntries.size(); ++i) {
		/*if (!m_meshEntries[i] || !m_meshEntries[i]->m_VAO) {
			std::cerr << "Error: MeshEntry or its VAO is not initialized!" << std::endl;
			continue;
		}*/

		//m_meshEntries[i]->vao->bind();
		glBindVertexArray(m_meshEntries[i]->vao);

		// Kontrola, že všechny VBO v m_meshEntries jsou inicializovány
		/*for (const auto& vbo : m_meshEntries[i]->m_VBO) {
			if (!vbo) {
				std::cerr << "Error: VBO in MeshEntry is not initialized!" << std::endl;
				continue;
			}
		}*/

		/*m_meshEntries[i]->m_VBO[VERTEX_BUFFER]->bind();
		m_meshEntries[i]->m_VBO[TEXCOORD_BUFFER]->bind();
		m_meshEntries[i]->m_VBO[NORMAL_BUFFER]->bind();
		m_meshEntries[i]->m_VBO[INDEX_BUFFER]->bind();*/
		glBindBuffer(GL_ARRAY_BUFFER, m_meshEntries[i]->vbo[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, m_meshEntries[i]->vbo[TEXCOORD_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, m_meshEntries[i]->vbo[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, m_meshEntries[i]->vbo[INDEX_BUFFER]);

		if (m_material.size() - 1 >= i) {
			//m_shader.sendUniform("meshMaterial.ambient", glm::vec3(1.0f, 1.0f, 1.0f)/*m_material[i]->ambient*/);
			//m_shader.sendUniform("meshMaterial.diffuse", m_material[i]->diffuse);
			//m_shader.sendUniform("meshMaterial.specular", m_material[i]->specular);

			if (m_material[i]->diffuseMap == "no_texture") {
				//m_shader.sendUniform("hasTexture", 0);
			}
			else {
				//m_shader.sendUniform("hasTexture", 1);
				
				glBindTexture(GL_TEXTURE_2D, t[textureNumberToUse]->getTextureId());
				++textureNumberToUse;
			}
			//printf("diffuse: %f, %f, %f\n", material[i]->diffuse.x, material[i]->diffuse.y, material[i]->diffuse.z);
			
		}

		//m_meshEntries.at(i)->render();
		// Vykreslovací kód (napø. glDrawArrays nebo glDrawElements)
		
		//glDrawElements(GL_TRIANGLES, m_meshEntries[i]->m_numVertices, GL_UNSIGNED_INT, NULL); // uncomment, actually broken

		// Kontrola, že poèet vrcholù je správný
		if (m_meshEntries[i]->m_numVertices <= 0) {
			std::cerr << "Error: Number of vertices is invalid!" << std::endl;
			continue;
		}

		// Vykreslovací kód (napø. glDrawArrays nebo glDrawElements)
		glDrawElements(GL_TRIANGLES, m_meshEntries[i]->m_numVertices * 3 * sizeof(GLfloat), GL_UNSIGNED_INT, NULL);

		glBindVertexArray(0);
		//m_meshEntries[i]->m_VAO->unbind();
	}

	//m_VAO->unbind();
}

int Mesh::getTextureCount() const
{
	return m_textures.size();
}

void Mesh::setShader(Shader shader)
{
	m_shader = shader;
}
