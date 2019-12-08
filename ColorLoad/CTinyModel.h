#pragma once
#define TINYOBJLOADER_IMPLEMENTATION
#include<tiny_obj_loader.h>
#include<vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include<thread>
#include"CMesh.h"

class CTinyModel
{
public:	
	void showModel(std::string vPath,  bool vIsTri = true)
	{
		tinyobj::attrib_t Attrib;
		std::vector<tinyobj::shape_t> Shape;
		std::vector<tinyobj::material_t> Material;
		std::string Warn;
		std::string Err;
		std::string BasePath = vPath.substr(0, vPath.find_last_of('/'));
		bool IsSuccessed = tinyobj::LoadObj(&Attrib, &Shape, &Material, &Warn, &Err, vPath.c_str(), BasePath.c_str(), vIsTri);
		if (!Warn.empty()) 
		{
			std::cout << "WARN: " << Warn << std::endl;
		}

		if (!Err.empty()) 
		{
			std::cerr << "ERR: " << Err << std::endl;
		}

		if (!IsSuccessed)
		{
			std::cout << "FAIL TO LOAD" << std::endl;
		}
		std::cout << "Vertices: " << Attrib.vertices.size() / 3<<std::endl;
		std::cout << "Normal: " << Attrib.normals.size() / 3<< std::endl;
		std::cout << "Texcoords: " << Attrib.texcoords.size() / 2<< std::endl;
		std::cout << "Shape: " << Shape.size() << std::endl;
		int VerticesAmount = 0;
		for (int i = 0; i < Shape.size(); i++)
		{
			
			std::cout << "Mesh " << i <<" Name "<< Shape[i].name<< " indices " << Shape[i].mesh.indices.size() << std::endl;
			std::cout << "      " << " Num_face_vertices " << Shape[i].mesh.num_face_vertices.size() << std::endl;
			if (i == 0)
			{
				/*for (int k = 0; k < Shape[i].mesh.num_face_vertices.size(); k++)
				{
					std::cout << (int)Shape[i].mesh.num_face_vertices[k] << std::endl;
				}*/
				for (int k = 0; k < Shape[i].mesh.indices.size(); k++)
				{
					std::cout<< k << " VertexID " << Shape[i].mesh.indices[k].vertex_index << " NormalID " << Shape[i].mesh.indices[k].normal_index <<
						" TexID " << Shape[i].mesh.indices[k].texcoord_index << std::endl;
					std::cout << "Vertex position " << Attrib.vertices[Shape[i].mesh.indices[k].vertex_index] << std::endl;
				}
			}
			VerticesAmount += Shape[i].mesh.indices.size();
			/*std::cout << "indices: " << std::endl;
			for (int k = 0; k < Shape[i].mesh.indices.size(); k++)
			{
				std::cout << Shape[i].mesh.indices[k].vertex_index<<std::endl;
			}*/
		}
		std::cout << "VerticeAmount " << VerticesAmount << std::endl;
	}
	void showMaterial()
	{
		for (int i = 0; i < m_Materials.size(); i++)
		{
			std::cout << "m_Material " << i << std::endl;
			std::cout << m_Materials[i].Textures[0].ID << std::endl;
			std::cout << m_Materials[i].Textures[1].ID << std::endl;
			std::cout << m_Materials[i].Textures[2].ID << std::endl;
			std::cout << std::endl;
		}
	}
	void showBox()
	{
		for (int i = 0; i < m_Meshes.size(); i++)
		{
			std::cout << i << ":" << std::endl;
			m_Meshes[i].showBox();
		}
	}
	void showMesh()
	{
		for (int i = 0; i < m_Meshes.size(); i++)
		{
			std::cout << "Mesh " << i << std::endl;
			m_Meshes[i].showDetail();
		}
	}
	CTinyModel(std::string vPath)
	{
		loadModel(vPath);
		std::cout << m_Meshes[0].getVertices().size() << std::endl;		
	}
	void loadModel(std::string vPath)
	{
		m_Directory = vPath.substr(0, vPath.find_last_of('/'));
		
		tinyobj::attrib_t Attrib;
		std::vector<tinyobj::shape_t> Shape;
		std::vector<tinyobj::material_t> Material;
		std::string Warn;
		std::string Error;
		bool IsLoaded = tinyobj::LoadObj(&Attrib, &Shape, &Material, &Warn, &Error, vPath.c_str(), m_Directory.c_str());
		if (!Warn.empty())
			std::cout << "WARN: " << Warn << std::endl;
		if (!Error.empty())
			std::cerr << "ERROR: " << Error << std::endl;
		if (!IsLoaded)
		{
			std::cout << "FAIL TO LOAD " << std::endl;
			return;
		}
		else
			std::cout << "success load" << std::endl;
			__processModel(Attrib, Shape, Material);
		
	}
	void changeTexToColor()
	{
		for (unsigned int i = 0; i < m_Meshes.size(); i++)
			m_Meshes[i].changeTexToColor(m_Directory);
	}
	void matchColor(CTinyModel vModel)
	{
		std::cout << "match color start" << std::endl;
		
			
		m_Meshes[0].findColor(vModel.getMeshes());			
			
	}
	
	void matchVertex(CTinyModel vModel)
	{
		for (int i = 0; i < m_Meshes.size(); i++)
		{
			for (int k = 0; k < m_Meshes[i].getVertices().size(); k++)
			{
				matchPointInModel(m_Meshes[i].fetchVertices()[k], vModel);
				
			}
		}
	}
	void matchVertexForMultiThread(CTinyModel vModel, int vStart, int vEnd)
	{
		for (int i = vStart; i < vEnd; i++)
		{
			matchPointInModel(m_Meshes[0].fetchVertices()[i], vModel);
			if (i % 100 == 0)
				std::cout << i << " done" << std::endl;
		}
	}
	
	void matchPointInModel(SVertex& vVertex, CTinyModel vModel)
	{
		for (int i = 0; i < vModel.getMeshes().size(); i++)
		{
			matchPointInMesh(vVertex, vModel.getMeshes()[i]);
		}
	}
	
	void matchPointInMesh(SVertex& vVertex, CMesh vMesh)
	{
		for (int i = 0; i < vMesh.getVertices().size(); i++)
			vVertex.findColor(vMesh.getVertices()[i]);
	}

	void calColor()
	{
		for (int i = 0; i < m_Meshes.size(); i++)
		{
			for (int k = 0; k < m_Meshes[i].getVertices().size(); k++)
				m_Meshes[i].fetchVertices()[k].calColor();
		}
	}
	void setMeshes()
	{
		for (int i = 0; i < m_Meshes.size(); i++)
		{
			m_Meshes[i].setMesh();
		}
	}
	const std::vector<CMesh>& getMeshes() const
	{
		return m_Meshes;
	}
	void showColor()
	{
		for (unsigned int i = 0; i < m_Meshes.size(); i++)
			m_Meshes[i].showColor();
	}
	void drawInColor(Shader vShader)
	{
		for (unsigned int i = 0; i < m_Meshes.size(); i++)
			m_Meshes[i].drawInColor(vShader);
	}
	void drawInTex(Shader vShader)
	{
		for (unsigned int i = 0; i < m_Meshes.size(); i++)
			m_Meshes[i].drawInTex(vShader);
	}
	std::vector<SVertex> processModel(CTinyModel vModel)
	{
		if (vModel.m_Meshes.size() > 1)
		{
			std::vector<SVertex> Vertices;
			for (int i = 0; i < m_Meshes.size(); i++)
			{
				Vertices.insert(Vertices.end(), m_Meshes[i].getVertices().begin(), m_Meshes[i].getVertices().end());
			}
			return Vertices;
		}
		else
			return m_Meshes[0].getVertices();
	}
	
private:
	std::vector<CMesh> m_Meshes;
	std::vector<STexture> m_TextureLoaded;
	std::vector<SMaterial> m_Materials;	
	std::string m_Directory;
	void __processModel(tinyobj::attrib_t vAttrib, std::vector<tinyobj::shape_t> vShapes, std::vector<tinyobj::material_t> vMaterials)
	{
		for (int i = 0; i < vMaterials.size(); i++)
		{
			SMaterial Material;			
			STexture DiffTex("texture_diffuse");
			STexture SpecTex("texture_specular");
			STexture BumpTex("texture_height");
			DiffTex.Path = vMaterials[i].diffuse_texname;
			SpecTex.Path = vMaterials[i].specular_texname;
			BumpTex.Path = vMaterials[i].bump_texname;
			Material.Textures.push_back(DiffTex);
			Material.Textures.push_back(SpecTex);
			Material.Textures.push_back(BumpTex);
			Material.Name = vMaterials[i].name;
			m_Materials.push_back(Material);
		}	
		__loadTextures();		
		for (int i = 0; i < vShapes.size(); i++)
		{
			__processMesh(vAttrib, vShapes[i]);			
		}
	}
	void __processMesh(tinyobj::attrib_t vAttrib, tinyobj::shape_t vShape)
	{
		std::vector<SVertex> Vertices;
		std::vector<unsigned int> Indices;
		std::vector<STexture> Textures;
		for (int i = 0; i < vShape.mesh.indices.size(); i++)
		{
			tinyobj::index_t Index = vShape.mesh.indices[i];			
			SVertex Vertex;
			glm::vec3 Position;
			if (Index.vertex_index != -1)
			{
				Position.x = vAttrib.vertices[3 * Index.vertex_index];
				Position.y = vAttrib.vertices[3 * Index.vertex_index + 1];
				Position.z = vAttrib.vertices[3 * Index.vertex_index + 2];
			}
			else
				Position = glm::vec3(0, 0, 0);
			glm::vec3 Normal;
			if (Index.normal_index != -1)
			{
				Normal.x = vAttrib.normals[3 * Index.normal_index];
				Normal.y = vAttrib.normals[3 * Index.normal_index + 1];
				Normal.z = vAttrib.normals[3 * Index.normal_index + 2];
			}
			else
				Normal = glm::vec3(0, 0, 0);
			glm::vec2 TexCoords;
			if (Index.texcoord_index != -1)
			{
				TexCoords.x = vAttrib.texcoords[2 * Index.texcoord_index];
				TexCoords.y = vAttrib.texcoords[2 * Index.texcoord_index + 1];
			}
			else
				TexCoords = glm::vec2(0, 0);
			Vertex.Position = Position;
			Vertex.Normal = Normal;
			Vertex.TexCoords = TexCoords;			
			Vertices.push_back(Vertex);
			
		}		
		for (int i = 0; i < vShape.mesh.indices.size(); i++)
		{
			Indices.push_back(i);
			 
		}
		Textures = __loadMaterial(vShape);
		CMesh Mesh(Vertices, Indices, Textures);
		m_Meshes.push_back(Mesh);
	}
	std::vector<STexture> __loadMaterial(tinyobj::shape_t vShape)
	{
		int ID = vShape.mesh.material_ids[0];	
		std::vector<STexture> Textures;
		if (ID == -1)
			return Textures;
		STexture DiffTex = m_Materials[ID].Textures[0];		
		STexture SpecTex = m_Materials[ID].Textures[1];
		STexture BumpTex = m_Materials[ID].Textures[2];
		Textures.push_back(DiffTex);
		Textures.push_back(SpecTex);
		Textures.push_back(BumpTex);
		return Textures;
	}
	void __loadTextures()
	{
		for (int i = 0; i < m_Materials.size(); i++)
		{
			for (int k = 0; k < m_Materials[i].Textures.size(); k++)
			{
				if (!m_Materials[i].Textures[k].Path.empty())
				{
					std::string Path = m_Directory + '/' + m_Materials[i].Textures[k].Path;
					unsigned int TextureID;
					glGenTextures(1, &TextureID);
					
					m_Materials[i].Textures[k].ID = TextureID;
								
					unsigned char* Data = stbi_load(Path.c_str(), &m_Materials[i].Textures[k].Width,
						&m_Materials[i].Textures[k].Height, &m_Materials[i].Textures[k].NrComponents, 0);
					if (Data)
					{
						GLenum format;
						if (m_Materials[i].Textures[k].NrComponents == 1)
							format = GL_RED;
						else if (m_Materials[i].Textures[k].NrComponents == 3)
							format = GL_RGB;
						else if (m_Materials[i].Textures[k].NrComponents == 4)
							format = GL_RGBA;

						glBindTexture(GL_TEXTURE_2D, TextureID);
						glTexImage2D(GL_TEXTURE_2D, 0, format, m_Materials[i].Textures[k].Width,
							m_Materials[i].Textures[k].Height, 0, format, GL_UNSIGNED_BYTE, Data);
						glGenerateMipmap(GL_TEXTURE_2D);

						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

						stbi_image_free(Data);
					}
					else
					{
						std::cout << "Texture failed to load at path: " << Path << std::endl;
						stbi_image_free(Data);
					}
					m_TextureLoaded.push_back(m_Materials[i].Textures[k]);
				}
				else
					m_Materials[i].Textures[k].ID = 0;

			}
		}
		
	}
	
};

void loadexample(std::string vInput)
{
	
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, vInput.c_str());

	if (!warn.empty()) {
		std::cout << warn << std::endl;
	}

	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	if (!ret) {
		exit(1);
	}

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
				tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
				tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
				tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
				tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
				tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
				// Optional: vertex colors
				// tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
				// tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
				// tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}
}