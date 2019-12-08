#pragma once
#include "CTinyModel.h"

class CHandleModel
{
public:
	CHandleModel(CTinyModel vModel)
	{
		
		for (int i = 0; i < vModel.getMeshes().size(); i++)
		{
			CMesh Mesh = vModel.getMeshes()[i];
			m_Vertices.insert(m_Vertices.end(), Mesh.getVertices().begin(), Mesh.getVertices().end());
		}
	}
	std::vector<SVertex> getVertices()
	{
		return m_Vertices;
	}
private:
	
	std::vector<SVertex> m_Vertices;
};
