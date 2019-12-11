#include "COctree.h"
COctree::COctree(std::vector<SVertex> vVertices, SBox vBox, int vMaxVerticesPerNode, std::vector<SVertex*> vReVertices)
{
	m_Vertices = vVertices;
	m_MaxVerticesPerNode = vMaxVerticesPerNode;
	m_ReVertices = vReVertices;
	m_Root = new SNode(vBox, 0);
	m_Root->Vertices = m_Vertices;
	m_Root->ReVertices = vReVertices;
	m_NumOfVertices = m_Vertices.size();
	__setChildren(m_Root);	
	__doCheck();
}

void COctree::__setChildren(SNode* vNode)
{			
	if ((!vNode->Vertices.empty())&&vNode->Vertices.size()>m_MaxVerticesPerNode)
	{
		std::vector<SBox> Boxs = __setBox(vNode->Box);
		for (int i = 0; i < Oct; i++)
		{
			vNode->Children.push_back(new SNode(Boxs[i], vNode->Depth + 1));
		}
		for(int i = 0;i<vNode->Vertices.size();i++)
			for (int k = 0; k < Oct; k++)
			{
				if (vNode->Vertices[i].isInBox(vNode->Children[k]->Box))
				{
					vNode->Children[k]->Vertices.push_back(vNode->Vertices[i]);					
				}
			}
		for (int i = 0; i < Oct; i++)
		{
			__setChildren(vNode->Children[i]);
		}
	}
}
void COctree::putVertices(SNode* vNode)
{
	if (!vNode->ReVertices.empty() && !vNode->Children.empty() && !vNode->Vertices.empty())
	{
		for (int i = 0; i < vNode->ReVertices.size(); i++)
		{
			for (int k = 0; k < Oct; k++)
			{
				if (vNode->ReVertices[i]->isInBox(vNode->Children[k]->Box)&& !vNode->Children[k]->Vertices.empty())					
						vNode->Children[k]->ReVertices.push_back(vNode->ReVertices[i]);
			}
		}
		for (int i = 0; i < Oct; i++)
		{
			putVertices(vNode->Children[i]);
		}
	}
	if (!vNode->ReVertices.empty() && !vNode->Vertices.empty())
	{
		if(vNode->Children.empty())
		for (int i = 0; i < vNode->ReVertices.size(); i++)
			vNode->ReVertices[i]->findVertexInVector(vNode->Vertices);
		else if (!vNode->Children.empty())
		{
			for (int i = 0; i < Oct; i++)
			{
				if (!vNode->Children[i]->ReVertices.empty())
					return;
			}
			for (int i = 0; i < vNode->ReVertices.size(); i++)
				vNode->ReVertices[i]->findVertexInVector(vNode->Vertices);
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
	m_NumOfVerticesInNode = 0;
	showNode(m_Root);
	std::cout << "Vertices divided into Nodes " << m_NumOfVerticesInNode << std::endl;
	std::cout << "MaxDepth " << m_MaxDepth << std::endl;
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
	if (vNode->Children.empty())
	{
		std::cout << "Box : Depth " << vNode->Depth << std::endl;
		std::cout << "(" << vNode->Box.MaxX << "," << vNode->Box.MinX << ") ";
		std::cout << "(" << vNode->Box.MaxY << "," << vNode->Box.MinY << ") ";
		std::cout << "(" << vNode->Box.MaxZ << "," << vNode->Box.MinZ << ")" << std::endl;
		std::cout << "Vertices :" << vNode->Vertices.size() << std::endl;
		m_NumOfVerticesInNode += vNode->Vertices.size();
		if (vNode->Depth > m_MaxDepth)
			m_MaxDepth = vNode->Depth;
		std::cout << "<<<<<<<<<<<<<<<<<<<<" << std::endl;
	}
}

void COctree::__setVertices()
{
	for (int i = 0; i < m_Vertices.size(); i++)
	{
		__putVertex(m_Root, m_Vertices[i]);
	}
}

void COctree::__putVertex(SNode* vNode, SVertex& vVertex)
{
	if (vVertex.isInBox(vNode->Box))
	{
		vNode->Vertices.push_back(vVertex);		
		if (!vNode->Children.empty())
		{
			for (int i = 0; i < Oct; i++)
			{
				__putVertex(vNode->Children[i], vVertex);
			}
		}
	}
}
void COctree::__check(SNode* vNode)
{
	if (!vNode->Children.empty())
	{
		for (int i = 0; i < Oct; i++)
		{
			__check(vNode->Children[i]);
		}
	}
	else
	{
		m_NumOfVerticesInNode += vNode->Vertices.size();
		if (m_MaxDepth < vNode->Depth)
			m_MaxDepth = vNode->Depth;
	}
}
void COctree::__doCheck()
{
	__check(m_Root);
	if (m_NumOfVertices != m_NumOfVerticesInNode)
		std::cout << "Vertices do not match " << std::endl;
	else
		std::cout << "Octree tree building done " << std::endl;
}
void COctree::__match(SVertex* vVertex, SNode* vNode)
{
	if (vVertex->isInBox(vNode->Box))
	{				
		if (!vNode->Vertices.empty())
		{					
			if (!vNode->Children.empty())
			{
				if (!vNode->isInChildren(vVertex)) 
				{
					vVertex->findVertexInVector(vNode->Vertices);
					return;
				}
				for (int i = 0; i < Oct; i++)
				{					
					__match(vVertex, vNode->Children[i]);					
				}
			}
			else
				vVertex->findVertexInVector(vNode->Vertices);
		}
	
	}
	else if (vNode->Vertices.size() == m_Vertices.size())
	{
		vVertex->findVertexInVector(m_Vertices);
	}
	
}
void COctree::doMatch(SVertex* vVertex)
{
	__match(vVertex, m_Root);
}
	