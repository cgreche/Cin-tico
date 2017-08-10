#pragma once

#ifndef __RENDERENGINEHELPER_H__
#define __RENDERENGINEHELPER_H__

#include "vertex3.h"

class RenderEngine;

class RenderEngineHelper
{
	RenderEngine &m_renderEngine;

public:
	RenderEngineHelper(RenderEngine &renderEngine): m_renderEngine(renderEngine) { }
	int createQuad(float width, float length);
	int createTriangle(float baseLength, float height);
	int createCircle(float radius, unsigned int numPoints, Color *colors);
	int createSquare(float sideLength);
	int createRectangularPrism(float width, float height, float length);
	int createTriangularPrism();
	int createSphere();
	int createCube(float edgeLength);
	void generateTerrain(float squareSize, int terrainGridWidth, int terrainGridHeight, Vertex3 **ppVertices, int **ppIndices, int *pVertexCount, int *pIndexCount);
	int loadSprite();
	int createText(const char *text);
};

#endif