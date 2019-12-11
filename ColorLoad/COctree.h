#pragma once
#include "CMesh.h"
const int Oct = 8;
struct SNode
{
	SBox Box;
	int Depth;
	std::vector<SVertex> Vertices;
	std::vector<SVertex*> ReVertices;
	std::vector<SNode*> Children;
	bool isLeafForRe = false;
	SNode(SBox vBox, int vDepth)
	{
		Box = vBox;
		Depth = vDepth;
	}
	SNode() {};
	void show()
	{
		std::cout << Vertices.size() << std::endl;
	}
	bool isInChildren(SVertex* vVertex)
	{
		for (int i = 0; i < Oct; i++)
		{
			if (!Children[i]->Vertices.empty() && vVertex->isInBox(Children[i]->Box))
				return true;
		}
		return false;
	}
};
class COctree
{
public:
	COctree(std::vector<SVertex> vVertices, SBox vBox, int vMaxVerticesPerNode, std::vector<SVertex*> vReVertices);
	void showNode(SNode* vNode);
	void show();
	void doMatch(SVertex* vVertex);
	void setReVertices(std::vector<SVertex*> vVertices)
	{
		m_ReVertices = vVertices;
	}
	void doPut()
	{
		putVertices(m_Root);
	}
	void putVertices(SNode* vNode);
	
private:
	std::vector<SVertex> m_Vertices;
	std::vector<SVertex*> m_ReVertices;
	SNode* m_Root;
	int m_MaxDepth;
	int m_NumOfNode = 0;
	int m_MaxVerticesPerNode;
	int m_NumOfVertices;
	int m_NumOfVerticesInNode;
	void  __setChildren(SNode* vNode);
	std::vector<SBox> __setBox(SBox vBox);
	void printNode(SNode* vNode);
	void __setVertices();
	void __putVertex(SNode* vNode, SVertex& vVertex);
	void __check(SNode* vNode);
	void __doCheck();
	void __match(SVertex* vVertex, SNode* vNode);
};
