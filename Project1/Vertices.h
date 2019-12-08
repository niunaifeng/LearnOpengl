#pragma once
#include <vector>
#include "stb_image.h"

#include "Shader.h"

struct SVertex
{
	glm::vec3 Position;
	glm::vec2 TexCoords;
	glm::vec3 Color;

};
struct STexture
{
	const char* Path;
	int Width;
	int Height;
	int NrComponents;
};
class CMesh
{
public:
	CMesh(std::vector<glm::vec3> vPosition, std::vector<glm::vec2> vTexcoord, std::vector<unsigned int> vIndices, const char* vTexture)
	{
		if (vPosition.size() != vTexcoord.size())
		{
			std::cout << "Input vertices information error !" << std::endl;
			return;
		}
		for (int i = 0; i < vPosition.size(); i++)
		{
			SVertex ThisVertex;
			ThisVertex.Position = vPosition[i];
			ThisVertex.TexCoords = vTexcoord[i];
			m_Vertices.push_back(ThisVertex);
		}
		m_Indices = vIndices;
		__storeColor(vTexture);
		__change();
		//__setMesh();
	}
	void ShowColor()
	{
		for (int i = 0; i < m_Color.size(); i++)
		{
			std::cout << "line " << i << std::endl;

			for (int k = 0; k < m_Color[i].size(); k++)
			{
				std::cout << "(";
				std::cout << m_Color[i][k].x << ",";
				std::cout << m_Color[i][k].y << ",";
				std::cout << m_Color[i][k].z << ")" << std::endl;
			}
			std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
		}
	}
	void showVertex()
	{
		for (int i = 0; i < m_Vertices.size(); i++)
		{
			std::cout << "Position:" << m_Vertices[i].Position.x << "," << m_Vertices[i].Position.y << "," << m_Vertices[i].Position.z << std::endl;
			std::cout << "Texcoords:" << m_Vertices[i].TexCoords.x << "," << m_Vertices[i].TexCoords.y << std::endl;
			std::cout << "Color:" << m_Vertices[i].Color.x << "," << m_Vertices[i].Color.y << "," << m_Vertices[i].Color.z << std::endl;
			std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
		}
	}
	void drawInTex();
	void drawInColor();
	unsigned int VAO;
private:
	std::vector<SVertex> m_Vertices;
	std::vector<std::vector<glm::vec3>> m_Color;
	std::vector<unsigned int> m_Indices;
	STexture m_Texture;
	unsigned int VBO;
	unsigned int EBO;
	void __change()
	{
		for (int i = 0; i < m_Vertices.size(); i++)
		{
			int x = (m_Texture.Width - 1) * m_Vertices[i].TexCoords.x;
			int y = (m_Texture.Height - 1) * m_Vertices[i].TexCoords.y;
			m_Vertices[i].Color = m_Color[y][x];
		}
	}
	void __storeColor(const char* vTexture)
	{
		m_Texture.Path = vTexture;
		unsigned char* Data = stbi_load(vTexture, &m_Texture.Width, &m_Texture.Height, &m_Texture.NrComponents, 0);
		if (!Data)
			std::cout << "failed to read" << std::endl;
		else
			std::cout << "successed" << std::endl;
		int Step = m_Texture.Width * m_Texture.NrComponents;
		for (int y = 0; y < m_Texture.Height; y++)
		{
			std::vector<glm::vec3> ThisLine;
			unsigned char* p = Data + (y * Step);
			for (int x = 0; x < m_Texture.Width; x++)
			{
				glm::vec3 ThisColor;
				ThisColor.x = (float)p[0];
				ThisColor.y = (float)p[1];
				ThisColor.z = (float)p[2];

				ThisLine.push_back(ThisColor);
				p += m_Texture.NrComponents;
			}
			m_Color.push_back(ThisLine);
		}
	}
	void __setMesh()
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
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)offsetof(SVertex, TexCoords));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)offsetof(SVertex, Color));

		glBindVertexArray(0);
	}
};
