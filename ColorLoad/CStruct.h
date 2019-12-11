#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <math.h>
#include <map>

struct SBox
{
	float MaxX;
	float MinX;
	float MaxY;
	float MinY;
	float MaxZ;
	float MinZ;
	float MAGNIFICATION = 1.05f;
	SBox() {};
	SBox(float vMaxX, float vMinX, float vMaxY, float vMinY, float vMaxZ, float vMinZ)
	{
		MaxX = vMaxX;
		MinX = vMinX;
		MaxY = vMaxY;
		MinY = vMinY;
		MaxZ = vMaxZ;
		MinZ = vMinZ;
	}
};
struct SVertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	glm::vec3 DiffColor;
	glm::vec3 SpecColor;
	glm::vec3 NormColor;
	glm::vec3 HeigColor;
	std::vector<SVertex> NN;
	bool isHandled = false;
	int MeshID;

	float MinDistance = 100.0f;
	SVertex() {};
	bool isInBox(SBox vBox)
	{
		return (Position.x >= vBox.MinX) && (Position.x <= vBox.MaxX) && (Position.y >= vBox.MinY) && (Position.y <= vBox.MaxY)
			&& (Position.z >= vBox.MinZ) && (Position.z <= vBox.MaxZ);
	}

	float calDistance(SVertex vVertex)
	{
		return sqrt(pow((Position.x - vVertex.Position.x), 2) + pow((Position.y - vVertex.Position.y), 2)
			+ pow((Position.z - vVertex.Position.z), 2));
	}
	void findVertex(SVertex vVertex)
	{
		float Distance = calDistance(vVertex);
		if (abs(Distance - MinDistance) < 0.01f)
		{
			NN.push_back(vVertex);
		}
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			NN.clear();
			NN.push_back(vVertex);
		}
	}
	void findVertexInVector(std::vector<SVertex>& vVertices)
	{
		for (int i = 0; i < vVertices.size(); i++)
		{
			float Distance = calDistance(vVertices[i]);
			if (abs(Distance - MinDistance) < 0.01f)
			{
				NN.push_back(vVertices[i]);
			}
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				NN.clear();
				NN.push_back(vVertices[i]);
			}
		}
		isHandled = true;
	}
	void calColor()
	{
		DiffColor = glm::vec3(0, 0, 0);
		for (int i = 0; i < NN.size(); i++)
		{
			DiffColor += NN[i].DiffColor;
		}
		DiffColor.x /= NN.size();
		DiffColor.y /= NN.size();
		DiffColor.z /= NN.size();
	}
	void calTexCoords()
	{
		glm::vec2 Tex(0,0);
		int Count = 0;
		for (int i = 0; i < NN.size(); i++)
		{
			if (NN[i].MeshID == MeshID)
			{
				Tex += NN[i].TexCoords;
				Count++;
			}
		}
		Tex.x /= Count;
		Tex.y /= Count;
		TexCoords = Tex;
	}
	void resetMeshID()
	{
		std::vector<int> MeshIDs;
		for (int i = 0; i < NN.size(); i++)
		{
			MeshIDs.push_back(NN[i].MeshID);
		}
		MeshID = findMostAppear(MeshIDs);
	}
	int findMostAppear(std::vector<int> vVector)
	{
		std::map<int, int> KeyList;
		std::vector<int>::iterator Viter;
		for (Viter = vVector.begin(); Viter != vVector.end(); Viter++)
		{
			KeyList[*Viter]++;
		}
		std::map<int, int>::iterator Miter;
		int Key = 0;
		int MaxValue = 0;
		for (Miter = KeyList.begin(); Miter != KeyList.end(); Miter++)
		{
			if (Miter->second > MaxValue)
			{
				MaxValue = Miter->second;
				Key = Miter->first;
			}
		}
		return Key;
	}
};
struct STexture
{
	unsigned int ID;
	std::string Path;
	std::string Type;
	int Width;
	int Height;
	int NrComponents;
	STexture() {};
	STexture(std::string vType)
	{
		Type = vType;
	}
};
struct SMaterial
{
	std::vector<STexture> Textures;
	std::string Name;
};
