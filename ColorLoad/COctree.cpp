#include "COctree.h"
COctree::COctree(std::vector<SVertex> vVertices, SBox vBox, int vMaxDepth)
{
	m_Vertices = vVertices;
	m_MaxDepth = vMaxDepth;
	m_Root = new SNode(vBox, 0);
	__setChildren(m_Root);
}

void COctree::__setChildren(SNode* vNode)
{
	SBox Box = vNode->Box;
	int Depth = vNode->Depth + 1;
	if (Depth < m_MaxDepth)
	{
		std::vector<SBox> Boxs = __setBox(vNode->Box);
		for (int i = 0; i < Oct; i++)
		{
			vNode->Children.push_back(new SNode(Boxs[i], Depth));
		}
		for (int i = 0; i < Oct; i++)
		{
			__setChildren(vNode->Children[i]);
		}
	}
}
std::vector<SBox> COctree::__setBox(SBox vBox)
{
	std::vector<SBox> Boxs;
	float MidX = (vBox.MaxX + vBox.MinX) / 2;
	float MidY = (vBox.MaxY + vBox.MinY) / 2;
	float MidZ = (vBox.MaxZ + vBox.MinZ) / 2;
	Boxs.push_back(SBox(vBox.MaxX, MidX, vBox.MaxY, MidY, MidZ, vBox.MinZ));
	Boxs.push_back(SBox(MidX, vBox.MinX, vBox.MaxY, MidY, MidZ, vBox.MinZ));
	Boxs.push_back(SBox(vBox.MaxX, MidX, MidY, vBox.MinY, MidZ, vBox.MinZ));
	Boxs.push_back(SBox(MidX, vBox.MinX, MidY, vBox.MinY, MidZ, vBox.MinZ));
	Boxs.push_back(SBox(vBox.MaxX, MidX, vBox.MaxY, MidY, vBox.MaxZ, MidZ));
	Boxs.push_back(SBox(MidX, vBox.MinX, vBox.MaxY, MidY, vBox.MaxZ, MidZ));
	Boxs.push_back(SBox(vBox.MaxX, MidX, MidY, vBox.MinY, vBox.MaxZ, MidZ));
	Boxs.push_back(SBox(MidX, vBox.MinX, MidY, vBox.MinY, vBox.MaxZ, MidZ));
	return Boxs;
}
void COctree::show()
{
	showNode(m_Root);
}
void COctree::showNode(SNode* vNode)
{
	if (!vNode->Children.empty())
	{
		for (int i = 0; i < Oct; i++)
			showNode(vNode->Children[i]);
	}
	std::cout << m_NumOfNode++ << std::endl;
	printNode(vNode);
}
void COctree::printNode(SNode* vNode)
{
	std::cout << "Box :" << std::endl;
	std::cout << "(" << vNode->Box.MaxX << "," << vNode->Box.MinX << ") ";
	std::cout << "(" << vNode->Box.MaxY << "," << vNode->Box.MinY << ") ";
	std::cout << "(" << vNode->Box.MaxZ << "," << vNode->Box.MinZ << ")" << std::endl;
	std::cout << "<<<<<<<<<<<<<<<<<<<<" << std::endl;

}