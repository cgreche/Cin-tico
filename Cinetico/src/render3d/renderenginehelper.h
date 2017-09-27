#pragma once

#ifndef __RENDERENGINEHELPER_H__
#define __RENDERENGINEHELPER_H__

#include "vertex3.h"

namespace render3d {

	class RenderEngine;

	class RenderEngineHelper
	{
		RenderEngine &m_renderEngine;

	public:
		RenderEngineHelper(RenderEngine &renderEngine) : m_renderEngine(renderEngine) { }
		int createQuad(float width, float length);
		int createTriangle(float baseLength, float height);
		int createCircle(float radius, unsigned int numPoints, Color *colors);
		int createSquare(float sideLength) { return createQuad(sideLength, sideLength); }

		int createRectangularPrism(float width, float height, float length, Color *colors);
		int createTriangularPrism();
		int createSphere();
		int createCube(float edgeLength, Color *colors) { return createRectangularPrism(edgeLength, edgeLength, edgeLength,colors); }
		int generateTerrain(float squareSize, int terrainGridWidth, int terrainGridHeight);

		int loadSprite();
		int createText(const char *text);
		int createText(const char *text, int x, int y, int width, int height);
	};

}

#endif