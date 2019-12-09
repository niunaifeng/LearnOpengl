#pragma once
//#include "CTinyModel.h"
#include "CMesh.h"
const int Oct = 8;
struct SNode
{
	SBox Box;
	int Depth;
	std::vector<SVertex> Vertices;
	std::vector<SNode*> Children;
	SNode(SBox vBox, int vDepth)
	{
		Box = vBox;
		Depth = vDepth;
	}
};
class COctree
{
public:
	COctree(std::vector<SVertex> vVertices, SBox vBox, int vMaxDepth);
	void showNode(SNode* vNode);
	void show();
private:
	std::vector<SVertex> m_Vertices;
	SNode* m_Root;
	int m_MaxDepth;
	int m_NumOfNode = 0;
	void __setNode(SNode* vNode);	
	void  __setChildren(SNode* vNode);
	std::vector<SBox> __setBox(SBox vBox);
	void printNode(SNode* vNode);
};
