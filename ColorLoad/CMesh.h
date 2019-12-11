#pragma once
#include "stb_image.h"
#include "Shader.h"
#include "CStruct.h"

class CMesh
{
public:
	CMesh(std::vector<SVertex> vVertices, std::vector<unsigned int> vIndices, std::vector<STexture> vTexture,std::string vName, std::string vMaterialName) 
	{
		
		m_Vertices = vVertices;
		m_Texture = vTexture;
		m_Indices = vIndices;				
		m_Name = vName;
		m_MaterialName = vMaterialName;
		setMesh();	
		__setBox();
	}
	CMesh() {};
	/*CMesh(CMesh& vMesh)
	{
		m_Texture = vMesh.m_Texture;
		m_Name = vMesh.m_Name;
		m_MaterialName = vMesh.m_MaterialName;
	}*/
	void drawInColor(Shader vShader)
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	void drawInTex(Shader shader)
	{
		// bind appropriate textures
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;		
		for (unsigned int i = 0; i < m_Texture.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
			// retrieve texture number (the N in diffuse_textureN)

			std::string number;
			std::string name = m_Texture[i].Type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++); // transfer unsigned int to stream
			else if (name == "texture_normal")
				number = std::to_string(normalNr++); // transfer unsigned int to stream
			else if (name == "texture_height")
				number = std::to_string(heightNr++); // transfer unsigned int to stream

													 // now set the sampler to the correct texture unit
			//glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
			shader.setInt((name + number).c_str(), i);
			// and finally bind the texture
			glBindTexture(GL_TEXTURE_2D, m_Texture[i].ID);
		}
		
		// draw mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// always good practice to set everything back to defaults once configured.
		glActiveTexture(GL_TEXTURE0);
	}
	void changeTexToColor(std::string vDirectory)
	{
		for (int i = 0; i < m_Texture.size(); i++)
		{
			if (!m_Texture[i].Path.empty())
			{
				std::string Path = vDirectory+ '/' + m_Texture[i].Path;
				unsigned char* Data = stbi_load(Path.c_str(), &m_Texture[i].Width, &m_Texture[i].Height, &m_Texture[i].NrComponents, 0);
				if (!Data)
					std::cout << "failed to read" << std::endl;


				int Step = m_Texture[i].Width * m_Texture[i].NrComponents;
				for (int k = 0; k < m_Vertices.size(); k++)
				{
					glm::vec3 Color;
					int x = (m_Texture[i].Width - 1) * m_Vertices[k].TexCoords.x;
					int y = (m_Texture[i].Height - 1) * m_Vertices[k].TexCoords.y;
					Color.x = (int)Data[Step * y + m_Texture[i].NrComponents * x] / 255.0f;
					Color.y = (int)Data[Step * y + m_Texture[i].NrComponents * x + 1] / 255.0f;
					Color.z = (int)Data[Step * y + m_Texture[i].NrComponents * x + 2] / 255.0f;
					if (m_Texture[i].Type == "texture_diffuse")
						m_Vertices[k].DiffColor = Color;
					else if (m_Texture[i].Type == "texture_specular")
						m_Vertices[k].SpecColor = Color;
					else if (m_Texture[i].Type == "texture_normal")
						m_Vertices[k].NormColor = Color;
					else if (m_Texture[i].Type == "texture_height")
						m_Vertices[k].HeigColor = Color;
					else
						std::cout << "Texture to color error" << std::endl;
				}

				stbi_image_free(Data);
			}
		}
		__setBox();
		setMesh();
	}
	void setMesh()
	{
		glGenVertexArrays(1, &VAO);

		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(SVertex), &m_Vertices[0], GL_STATIC_DRAW);
		glGenBuffers(1, &EBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)offsetof(SVertex, Normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)offsetof(SVertex, TexCoords));

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)offsetof(SVertex, Tangent));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)offsetof(SVertex, Bitangent));

		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)offsetof(SVertex, DiffColor));

		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)offsetof(SVertex, SpecColor));

		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)offsetof(SVertex, NormColor));

		glEnableVertexAttribArray(8);
		glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)offsetof(SVertex, HeigColor));

		glBindVertexArray(0);
	}
	SBox getBox()
	{
		return m_Box;
	}
	void findColor(std::vector<CMesh> vMesh)
	{				
		for (int i = 0; i < m_Vertices.size(); i++)
		{						
			for (int k = 0; k <vMesh.size(); k++)
			{
				for (int m = 0; m < vMesh[k].getVertices().size(); m++)
				{
					//if (m_Vertices[i].isInBox(vMesh[k].getBox()))
					m_Vertices[i].findVertex(vMesh[k].getVertices()[m]);
					
				}
			}
			
		}
			
	}
	void setMeshID(int ID)
	{
		for (int i = 0; i < m_Vertices.size(); i++)
		{
			m_Vertices[i].MeshID = ID;
		}
	}
	bool isInBox(SVertex vVertex)
	{
		return (vVertex.Position.x > m_Box.MinX) && (vVertex.Position.x < m_Box.MaxX) && (vVertex.Position.y > m_Box.MinY) && (vVertex.Position.y < m_Box.MaxY)
			&& (vVertex.Position.z > m_Box.MinZ) && (vVertex.Position.z < m_Box.MaxZ);
	}
	
	void changeVertex(SVertex vVertex)
	{
		vVertex.DiffColor = glm::vec3(0, 0, 0);
	}
	std::vector<SVertex>& getVertices()
	{
		return m_Vertices;
	}
	std::vector<SVertex>& fetchVertices() 
	{
		return m_Vertices;
	}
	std::vector<unsigned int>& fetchIndices()
	{
		return m_Indices;
	}
	const std::string getName()
	{
		return m_Name;
	}
	const std::string getMaterialName()
	{
		return m_MaterialName;
	}
	void showDetail()
	{
		std::cout << "Vertices :" << m_Vertices.size() << std::endl;
		std::cout << "Indices :" << m_Indices.size() << std::endl;
		std::cout << "name :" << m_Name << std::endl;
		std::cout << "material :" << m_MaterialName << std::endl;
		/*for (int i = 0; i < m_Indices.size(); i++)
		{
			std::cout << m_Indices[i] << std::endl;
		}
		std::cout << "<<<<<<<<<<<<<" << std::endl;*/
		/*for (int i = 0; i < m_Texture.size(); i++)
		{
			std::cout << m_Texture[i].Path << std::endl;
			std::cout << m_Texture[i].id << std::endl;
			std::cout << m_Texture[i].type << std::endl;
			std::cout << "<<<<<<<<<<<<<<<" << std::endl;
		}*/

		for (int i = 0; i < m_Vertices.size(); i++)
		{
		//	std::cout << "Diffues :" << m_Vertices[i].Position.x << "," << m_Vertices[i].Position.y << "," << m_Vertices[i].Position.z << std::endl;
		//	std::cout << "vertex " << i << std::endl;
		//	/*std::cout << "Position " << m_Vertices[i].Position << std::endl;
		//	std::cout << "TexCoords " << m_Vertices[i].TexCoords << std::endl;
		//	std::cout << "Diffuse Color " << m_Vertices[i].DiffColor << std::endl;
		//	std::cout << "Normal Color " << m_Vertices[i].NormColor << std::endl;*/
		//	std::cout << "Position:" << m_Vertices[i].Position.x << "," << m_Vertices[i].Position.y << "," << m_Vertices[i].Position.z << std::endl;
		//	std::cout << "Texcoords:" << m_Vertices[i].TexCoords.x << "," << m_Vertices[i].TexCoords.y << std::endl;
		//	std::cout << "Diffues :" << m_Vertices[i].DiffColor.x << "," << m_Vertices[i].DiffColor.y << "," << m_Vertices[i].DiffColor.z << std::endl;
		//	std::cout << "Specular :" << m_Vertices[i].SpecColor.x << "," << m_Vertices[i].SpecColor.y << "," << m_Vertices[i].SpecColor.z << std::endl;
		//	std::cout << "Normal :" << m_Vertices[i].NormColor.x << "," << m_Vertices[i].NormColor.y << "," << m_Vertices[i].NormColor.z << std::endl;
		//	std::cout << "Height :" << m_Vertices[i].HeigColor.x << "," << m_Vertices[i].HeigColor.y << "," << m_Vertices[i].HeigColor.z << std::endl;
		//	std::cout << "---------------"<<std::endl;
			
		}
	}
	void showTexture()
	{
		for (int i = 0; i < m_Texture.size(); i++)
		{
			std::cout << "m_Texture " << i << std::endl;
			std::cout << m_Texture[i].ID << std::endl;
			std::cout << m_Texture[i].Type << std::endl;
			std::cout << m_Texture[i].Path << std::endl;
		}
		std::cout << "<<<<<<<<<<<<<<<<<<<<<" << std::endl;
	}
	void showColor()
	{
		for (int i = 0; i < m_Vertices.size(); i++)
		{
			std::cout << "Vertex " << i << std::endl;
			std::cout << "(" << m_Vertices[i].DiffColor.x << "," << m_Vertices[i].DiffColor.y << "," << m_Vertices[i].DiffColor.z << ")" << std::endl;
		}
	}
	void showBox()
	{
		std::cout << "X " << m_Box.MinX << "--" << m_Box.MaxX << std::endl;
		std::cout << "Y " << m_Box.MinY << "--" << m_Box.MaxY << std::endl;
		std::cout << "Z " << m_Box.MinZ << "--" << m_Box.MaxZ << std::endl;
	}
	
private:	
	std::vector<SVertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<STexture> m_Texture;
	std::string m_Name;
	std::string m_MaterialName;
	SBox m_Box;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	
	void __setBox()
	{
		float MaxX, MinX, MaxY, MinY, MaxZ, MinZ;
		MaxX = MinX = m_Vertices[0].Position.x;
		MaxY = MinY = m_Vertices[0].Position.y;
		MaxZ = MinZ = m_Vertices[0].Position.z;
		for (int i = 1; i < m_Vertices.size(); i++)
		{
			if (m_Vertices[i].Position.x > MaxX)
				MaxX = m_Vertices[i].Position.x;
			if (m_Vertices[i].Position.x < MinX)
				MinX = m_Vertices[i].Position.x;
			if (m_Vertices[i].Position.y > MaxY)
				MaxY = m_Vertices[i].Position.y;
			if (m_Vertices[i].Position.y < MinY)
				MinY = m_Vertices[i].Position.y;
			if (m_Vertices[i].Position.z > MaxZ)
				MaxZ = m_Vertices[i].Position.z;
			if (m_Vertices[i].Position.z < MinZ)
				MinZ = m_Vertices[i].Position.z;
		}
		m_Box.MaxX = MaxX;
		m_Box.MinX = MinX;
		m_Box.MaxY = MaxY;
		m_Box.MinY = MinY;
		m_Box.MaxZ = MaxZ;
		m_Box.MinZ = MinZ;

	}
	
	
};

