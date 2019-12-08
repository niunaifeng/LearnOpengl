#pragma once



#include "CMesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

unsigned int TextureFromFile(const char *path, const std::string &m_Directory, bool gamma = false);

void showScene(const aiScene * vScene);

class CModel
{
public:
	std::vector<STexture> m_TextureLoaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<CMesh> m_Meshes;
	
	std::string m_Directory;
	bool gammaCorrection;

	/*  Functions   */
	// constructor, expects a filepath to a 3D model.
	CModel(std::string const &path, bool gamma = false) : gammaCorrection(gamma)
	{
		loadModel(path);		
	}

	// draws the model, and thus all its m_Meshes
	void drawInTex(Shader shader)
	{
		for (unsigned int i = 0; i < m_Meshes.size(); i++)
			m_Meshes[i].drawInTex(shader);
	}
	void drawInColor(Shader vShader)
	{
		for (unsigned int i = 0; i < m_Meshes.size(); i++)
			m_Meshes[i].drawInColor(vShader);
	}
	void changeTexToColor(std::string vDirectory)
	{
		for (unsigned int i = 0; i < m_Meshes.size(); i++)
			m_Meshes[i].changeTexToColor(vDirectory);
	}
private:

	void loadModel(std::string const &path)
	{
		// read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}
		// retrieve the m_Directory path of the filepath
		m_Directory = path.substr(0, path.find_last_of('/'));

		//process ASSIMP's root node recursively
		processNode(scene->mRootNode, scene);
		/*std::cout << scene->mNumMeshes << std::endl;
		std::cout << "texture" << scene->mNumTextures << std::endl;
		std::cout <<"Vertices: "<< scene->mMeshes[0]->mNumVertices << std::endl;
		
		std::cout <<"Num of Indices: "<< scene->mMeshes[0]->mFaces->mNumIndices << std::endl;*/
		/*for (int i = 0; i < scene->mMeshes[0]->mNumVertices; i++)
		{
			std::cout << i << ":(" << scene->mMeshes[0]->mVertices[i].x << "," << scene->mMeshes[0]->mVertices[i].y << "," << scene->mMeshes[0]->mVertices[i].z << ")" << std::endl;
		}*/
		/*for (int i = 0; i < scene->mMeshes[0]->mFaces->mNumIndices; i++)
		{
			std::cout <<i<<":"<< scene->mMeshes[0]->mFaces->mIndices[i] << std::endl;
		}*/
		
	}

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode *node, const aiScene *scene)
	{
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(processMesh(mesh, scene));
		}
		// after we've processed all of the m_Meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}

	}

	CMesh processMesh(aiMesh *mesh, const aiScene *scene)
	{
		// data to fill
		std::vector<SVertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<STexture> textures;

		// Walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{

			SVertex vertex;
			glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			// texture coordinates
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			// tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
			vertices.push_back(vertex);
			//// ‰≥ˆ”≥…‰
			//std::cout << "vertices: (" << vertex.Position.x << "," << vertex.Position.y << "," << vertex.Position.z << ")  ";
			//std::cout << "TexCoords: (" << vertex.TexCoords.x << "," << vertex.TexCoords.y << ")" << std::endl;
		}
		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// process materials
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

		// 1. diffuse maps
		std::vector<STexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<STexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<STexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<STexture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		// return a mesh object created from the extracted mesh data
		return CMesh(vertices, indices, textures);
	}

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	std::vector<STexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
	{
		std::vector<STexture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			bool skip = false;
			for (unsigned int j = 0; j < m_TextureLoaded.size(); j++)
			{
				if (std::strcmp(m_TextureLoaded[j].Path.data(), str.C_Str()) == 0)
				{
					textures.push_back(m_TextureLoaded[j]);
					skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{   // if texture hasn't been loaded already, load it
				STexture texture;
				texture.ID = TextureFromFile(str.C_Str(), this->m_Directory);
				
				texture.Type = typeName;
				texture.Path = str.C_Str();
				textures.push_back(texture);
				m_TextureLoaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}
		return textures;
	}
};

unsigned int TextureFromFile(const char *path, const std::string &m_Directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = m_Directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	
	return textureID;
}
void showScene(const aiScene *vScene)
{
	int TotalMeshes = vScene->mNumMeshes;
	int TotalTexture = vScene->mNumTextures;
	int TotalVertices = 0;
	int TotalFaces = 0;
	for (int i = 0; i < vScene->mNumMeshes; i++)
	{
		TotalVertices += vScene->mMeshes[i]->mNumVertices;
		TotalFaces += vScene->mMeshes[i]->mNumFaces;
		std::cout << "No" << i << " Mesh:" << vScene->mMeshes[i]->mNumVertices <<" vertices"<< std::endl;
		std::cout << "No" << i << " Mesh:" << vScene->mMeshes[i]->mNumFaces <<" faces"<< std::endl;
	}
	std::cout << "TotalMeshes " << TotalMeshes << std::endl;
	std::cout << "TotalTexture " << TotalTexture << std::endl;
	std::cout << "TotalVertices " << TotalVertices << std::endl;
	std::cout << "TotalFaces " << TotalFaces << std::endl;
}



