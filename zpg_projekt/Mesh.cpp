#include "Mesh.hpp"
#include "TextureManager.hpp"

#include <iostream>
#include <set>


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
	// Konstruktor přesune data do členských proměnných pomoci std::move()
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
void Mesh::render(bool f)
{
	/*for (int i = 0; i < m_meshEntries.size(); ++i) {
		// Zajištění, že všechny objekty jsou správně inicializovny
		if (!m_meshEntries[i]->vao) {
			std::cout << "Error: VAO is not initialized!" << std::endl;
			return;
		}
		// Kontrola, že vechny VBO jsou inicializovány
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
	for (size_t j = 0; j < m_material.size(); ++j) {
		if (m_material[j]->diffuseMap != "no_texture") {
			t.emplace_back(textureManager->getModelTexture(m_material[j]->diffuseMap));
		}
	}

	int textureNumberToUse = 0;

	for (size_t i = 0; i < m_meshEntries.size(); ++i) {
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
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshEntries[i]->vbo[INDEX_BUFFER]);

		if (i < m_material.size()) {
			//m_shader.sendUniform("meshMaterial.ambient", glm::vec3(1.0f, 1.0f, 1.0f)/*m_material[i]->ambient*/);
			//m_shader.sendUniform("meshMaterial.diffuse", m_material[i]->diffuse);
			//m_shader.sendUniform("meshMaterial.specular", m_material[i]->specular);

			if (m_material[i]->diffuseMap == "no_texture") {
				m_shader->sendUniform("hasTexture", 0);
			}
			else {
				m_shader->sendUniform("hasTexture", 1);
				glActiveTexture(GL_TEXTURE0 + textureNumberToUse);
				glBindTexture(GL_TEXTURE_2D, t[textureNumberToUse]->getTextureId());
				++textureNumberToUse;
			}
			//printf("diffuse: %f, %f, %f\n", material[i]->diffuse.x, material[i]->diffuse.y, material[i]->diffuse.z);
			
		}

		//m_meshEntries.at(i)->render();
		// Vykreslovac� k�d (nap�. glDrawArrays nebo glDrawElements)
		
		//glDrawElements(GL_TRIANGLES, m_meshEntries[i]->m_numVertices, GL_UNSIGNED_INT, NULL); // uncomment, actually broken

		// Kontrola, že počet vrcholů je správný
		if (m_meshEntries[i]->m_numVertices <= 0) {
			std::cerr << "Error: Number of vertices is invalid!" << std::endl;
			continue;
		}

		glDrawElements(GL_TRIANGLES, m_meshEntries[i]->elementCount, GL_UNSIGNED_INT, NULL);

		glBindVertexArray(0);
		//m_meshEntries[i]->m_VAO->unbind();
	}

	//m_VAO->unbind();
}

void Mesh::render() // nefunkcni
{
	auto textureManager = TextureManager::getInstance();
	static std::vector<std::shared_ptr<Texture>> t;
	//t.clear(); // Clear vector to reuse

	std::set<std::shared_ptr<Texture>> unique_textures;

	int num_texture_to_load = 0;

	glActiveTexture(GL_TEXTURE0 + 0);

	for (size_t j = 0; j < m_material.size(); ++j) {
		
		auto texture = textureManager->getModelTexture(m_material[j]->diffuseMap);
		t.emplace_back(texture);
		//unique_textures.insert(texture);

		//t.emplace_back(textureManager->getModelTexture("..\\" + material[j]->diffuseMap));
		//std::string a = "..\\" + material[j]->diffuseMap;
	}

	//auto t2 = t.erase(std::unique(t.begin(), t.end()), t.end());
	
	

	/*auto new_end = std::remove_if(t2.begin(), t2.end(),
		[&t2](const std::shared_ptr<Texture>& text)
		{
			auto it = std::find(t2.begin(), t2.end(), text->getName());
			return it != t2.end();
		});

	t2.erase(new_end, t2.end());
	*/


	for (size_t i = 0; i < m_meshEntries.size(); ++i) {
		if (i < m_material.size()) {
			glBindVertexArray(m_meshEntries[i]->vao);
            glBindBuffer(GL_ARRAY_BUFFER, m_meshEntries[i]->vbo[VERTEX_BUFFER]);
            glBindBuffer(GL_ARRAY_BUFFER, m_meshEntries[i]->vbo[TEXCOORD_BUFFER]);
            glBindBuffer(GL_ARRAY_BUFFER, m_meshEntries[i]->vbo[NORMAL_BUFFER]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshEntries[i]->vbo[INDEX_BUFFER]);
			//m_shader.sendUniform("meshMaterial.ambient", m_material[i]->ambient);
			//m_shader.sendUniform("meshMaterial.diffuse", m_material[i]->diffuse);
			//m_shader.sendUniform("meshMaterial.specular", m_material[i]->specular);

			if (m_material[i]->diffuseMap == "no_texture") {
				m_shader->sendUniform("hasTexture", 0);
			}
			else {
				m_shader->sendUniform("hasTexture", 1);

				auto texture = textureManager->getModelTexture(m_material[i]->diffuseMap);
				unique_textures.insert(texture);

				// added
				// Najít pořadí prvku v setu
				auto it = unique_textures.find(t[i]);
				if (it != unique_textures.end()) {
					// Iterujeme od začátku setu až po hledaný prvek
					auto index = std::distance(unique_textures.begin(), it);
					//std::cout << "Binding texture ID: " << it->getTextureId() << " at position " << index << std::endl;
					glBindTexture(GL_TEXTURE_2D + 0, (*it)->getTextureId());
					// glBindTexture(GL_TEXTURE_2D + num_texture_to_load++, it->getTextureId());
				}
				else {
					//std::cout << "Texture ID " << (*it)->getTextureId() << " not found in set." << std::endl;
				}

				/*auto it = unique_textures.find(t[i]);
				if (it != unique_textures.end()) {
					//int index = std::distance(unique_textures.begin(), it);
					
				}
				
				if (unique_textures.find(t[i]) != unique_textures.end()) {
					//std::cout << "Hodnota " << value << " je v setu." << std::endl;

				}*/

				// added 2024 - commented - originally uncommented
				//glBindTexture(GL_TEXTURE_2D + num_texture_to_load++, t[i]->getTextureId());
			}
			//printf("diffuse: %f, %f, %f\n", material[i]->diffuse.x, material[i]->diffuse.y, material[i]->diffuse.z);
			m_shader->sendUniform("meshMaterial.ambient", m_material[i]->ambient);
			m_shader->sendUniform("meshMaterial.diffuse", m_material[i]->diffuse);
			m_shader->sendUniform("meshMaterial.specular", m_material[i]->specular);
		}

            glDrawElements(GL_TRIANGLES, m_meshEntries[i]->elementCount, GL_UNSIGNED_INT, NULL);

		//glBindVertexArray(0);
	}
	//t.clear(); // added mb smazat
}

size_t Mesh::getTextureCount() const
{
	return m_textures.size();
}

void Mesh::setShader(std::shared_ptr<Shader> shader)
{
	m_shader = shader;
}
