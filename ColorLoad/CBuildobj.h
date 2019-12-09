#pragma once
#include <fstream>
#include "CTinyModel.h"
class CBuildobj
{
public:
	CBuildobj(CTinyModel vModel)
	{
		m_Model = vModel;
		buildModel(vModel);
	}
private:
	CTinyModel m_Model;
	std::ofstream Model;
	int m_VerticesNum = 0;
	void buildModel(CTinyModel vModel)
	{
		Model.open("rewrittenobj.txt");
		Model << "# this is a rewritten model" << std::endl;
		Model << "# to test txt transfer to obj" << std::endl;
		Model << "mtllib nanosuit.mtl" << std::endl;
		for (int i = 0; i < vModel.getMeshes().size(); i++)
		{
			buildMesh(vModel.getMeshes()[i]);
		}
	}
	void buildMesh(CMesh vMesh)
	{
		Model << "o " << vMesh.getName() << std::endl;
		for (int i = 0; i < vMesh.getVertices().size(); i++)
		{
			buildVertexPosition(vMesh.getVertices()[i]);
		}
		for (int i = 0; i < vMesh.getVertices().size(); i++)
		{
			buildVertexTexCoords(vMesh.getVertices()[i]);
		}
		for (int i = 0; i < vMesh.getVertices().size(); i++)
		{
			buildVertexNormal(vMesh.getVertices()[i]);
		}
		Model << "usemtl " << vMesh.getMaterialName() << std::endl;
		Model << "s 1" << std::endl;
		buildVertexFace(vMesh);
		m_VerticesNum += vMesh.getVertices().size();
	}
	void buildVertexPosition(SVertex vVertex)
	{
		Model << "v " << vVertex.Position.x << " " << vVertex.Position.y << " " << vVertex.Position.z << std::endl;
	}
	void buildVertexTexCoords(SVertex vVertex)
	{
		Model << "vt " << vVertex.TexCoords.x << " " << vVertex.TexCoords.y << std::endl;
	}
	void buildVertexNormal(SVertex vVertex)
	{
		Model << "vn " << vVertex.Normal.x << " " << vVertex.Normal.y << " " << vVertex.Normal.z << std::endl;
	}
	void buildVertexFace(CMesh vMesh)
	{
		for (int i = 0; i < vMesh.getVertices().size()/3; i++)
		{
			int Start = 3 * i + 1 + m_VerticesNum;
			Model << "f " << Start << "/" << Start << "/" << Start;
			Model << " " << Start + 1 << "/" << Start + 1 << "/" << Start + 1;
			Model << " " << Start + 2 << "/" << Start + 2 << "/" << Start + 2 << std::endl;
		}
	}

};
