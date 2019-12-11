#pragma once
#include"CTinyModel.h"
class CRebuiltModel
{
public:
	CRebuiltModel(CTinyModel vModel, std::vector<SVertex> vVertices)
	{
		m_Model = vModel;
		m_Vertices = vVertices;
		__clearModel();
		__resetModel();
	}
	CTinyModel& getModel()
	{
		return m_Model;
	}
private:
	CTinyModel m_Model;
	std::vector<SVertex> m_Vertices;
	void __clearModel()
	{
		for (int i = 0; i < m_Model.getMeshes().size(); i++)
		{
			m_Model.getMeshes()[i].fetchVertices().clear();
			m_Model.getMeshes()[i].fetchIndices().clear();
		}
	}
	void __resetModel()
	{
		for (int i = 0; i < m_Vertices.size(); i++)
		{
			int MeshID = m_Vertices[i].MeshID;
			m_Model.getMeshes()[MeshID].fetchVertices().push_back(m_Vertices[i]);
		}
		for (int i = 0; i < m_Model.getMeshes().size(); i++)
		{
			for (int k = 0; k < m_Model.getMeshes()[i].getVertices().size(); k++)
			{
				m_Model.getMeshes()[i].fetchIndices().push_back(k);
			}
		}
	}
};
