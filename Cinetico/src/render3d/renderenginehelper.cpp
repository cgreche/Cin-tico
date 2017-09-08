
#include <math.h>

#include "renderengine.h"
#include "renderenginehelper.h"

int RenderEngineHelper::createQuad(float width, float length) {
	float midW = width*0.5f;
	float midL = length*0.5f;
	
	Vertex3 quad[] = {
		{-midW, midL, 0}, {midW, midL, 0},
		{-midW,-midL, 0}, {midW,-midL, 0}
	};
	
	int quadIndices[] = { 0,1,2,2,1,3 };

	Color colors[] = {
		Color(255,255,255),
		Color(255,255,255),
		Color(255,255,255),
		Color(255,255,255)
	};
		
	return m_renderEngine.newResource(4, quad, 8, quadIndices, colors);
}

int RenderEngineHelper::createCircle(float radius, unsigned int numPoints, Color *colors) {
	const float PI = 3.14159f;

	unsigned int i;
	
	//Size of angle between two points on the circle (single wedge)
	float WedgeAngle;

	Vertex3 *circle = new Vertex3[numPoints];
	int *indices = new int[(numPoints-1)*3];

	//Precompute WedgeAngle
	WedgeAngle = (float)((2*PI) / (numPoints-1));

	//Set up vertices for a circle
	for(i = 0; i < numPoints-1; i++) {
		float x, y;
		float theta;

		//Calculate theta for this vertex
		theta = i * WedgeAngle;

		//Compute X and Y locations
		x = (float)(0.0f + radius * cos(theta));
		y = (float)(0.0f - radius * sin(theta));
		
		circle[i] = Vertex3(x, y, 1.0f);
	}
	circle[i] = Vertex3(0, 0, 1.f);

	//Fill indices
	int j = 0;
	for(i = 0; i < (numPoints-1)-1; i++, j += 3) {
		indices[j] = numPoints - 1;
		indices[j+1] = i;
		indices[j+2] = i + 1;
	}
	indices[j] = numPoints - 1;
	indices[j+1] = i;
	indices[j+2] = 0;

	int retId =  m_renderEngine.newResource(numPoints, circle, numPoints * 3, indices, colors);
	delete[] circle;
	delete[] indices;
	return retId;
}

int RenderEngineHelper::createRectangularPrism(float width, float height, float length)
{
	float hw = width / 2.f;
	float hh = height / 2.f;
	float hl = length / 2.f;

	Vertex3 prism[] = {
		//Front-Face
		{ -hw, hh, -hl }, { hw, hh, -hl },
		{ hw, -hh, -hl }, { -hw, -hh, -hl },

		//Back-Face
		{ -hw, hh, hl }, { hw, hh, hl },
		{ hw, -hh, hl }, { -hw, -hh, hl },

		//Up-Face
		{ -hw, hh, -hl }, { -hw, hh, hl },
		{ hw, hh, hl }, { hw, hh, -hl },

		//Down-Face
		{ -hw, -hh, hl }, { -hw, -hh, -hl },
		{ hw, -hh, -hl }, { hw, -hh, hl },

		//Left-Face
		{ -hw, -hh, hl }, { -hw, hh, hl },
		{ -hw, hh, -hl}, { -hw, -hh, -hl },

		//Right-Face
		{ hw, -hh, -hl }, { hw, hh, -hl },
		{ hw, hh, hl}, { hw, -hh, hl }
	};

	int prismIndices[] = {
		//Front-face
		0,1,2,2,3,0,
		//Back-face
		5,4,7,7,6,5,
		//Up-face
		8,9,10,10,11,8,
		//Down-face
		12,13,14,14,15,12,
		//Left-face
		16,17,18,18,19,16,
		//Right-face
		20,21,22,22,23,20
	};

	Color prismColors[] =
	{
		Color(255,0,0),
		Color(255,0,0),
		Color(255,0,0),
		Color(255,0,0),

		//Back
		Color(255,0,0),
		Color(255,0,0),
		Color(255,0,0),
		Color(255,0,0),

		//Top
		Color(0,0,0),
		Color(0,0,0),
		Color(0,0,0),
		Color(0,0,0),

		//Bottom
		Color(255,255,0),
		Color(255,255,0),
		Color(255,255,0),
		Color(255,255,0),

		//Left
		Color(255,255,255),
		Color(255,255,255),
		Color(255,255,255),
		Color(255,255,255),

		//Right
		Color(0,0,255),
		Color(0,0,255),
		Color(0,0,255),
		Color(0,0,255),
	};

	return m_renderEngine.newResource(24,prism,36,prismIndices,prismColors);

}

int RenderEngineHelper::generateTerrain(float squareSize, int terrainGridWidth, int terrainGridHeight)
{
	int squareCount = terrainGridWidth * terrainGridHeight;
	int vertexCount = squareCount * 4;
	int indexCount = squareCount * 6;
	int vertexPerRow = terrainGridWidth*4;
	Vertex3 *vertices = new Vertex3[vertexCount];
	int *indices = new int[indexCount];
	int i, j;
	int p;

	float x, y, z;
	y = 0.f;
	z = 0.f;
	p = 0;
	float width = squareSize*terrainGridWidth;
	float height = squareSize*terrainGridHeight;
	float xOrigin = -width / 2;
	float zOrigin = height / 2;
	for(i = 0; i < terrainGridHeight; ++i) {
		x = xOrigin;
		z = zOrigin - i * squareSize-1;
		for(j = 0; j < terrainGridWidth; ++j) {
			vertices[p++] = Vertex3(x, y, z);
			vertices[p++] = Vertex3(x+squareSize, y, z);
			vertices[p++] = Vertex3(x+squareSize, y, z-(squareSize));
			vertices[p++] = Vertex3(x, y, z-(squareSize));
			x += squareSize;
		}
	}

	p = 0;
	for (i = 0; i < terrainGridHeight; ++i) {
		for (j = 0; j < terrainGridWidth; ++j) {
			int topLeft = i*vertexPerRow + j*4;
			int topRight = topLeft + 1;
			int bottomRight = topRight + 1;
			int bottomLeft = bottomRight + 1;
			//int bottomLeft = topLeft + vertexPerRow;
			//int bottomRight = bottomLeft + 1;
			indices[p++] = bottomLeft;
			indices[p++] = topLeft;
			indices[p++] = topRight;
			indices[p++] = topRight;
			indices[p++] = bottomRight;
			indices[p++] = bottomLeft;
		}
	}

	
	Color * terrainColors = new Color[vertexCount];

	int s = 0;
	for (i = 0; i < terrainGridHeight; ++i) {

		Color color1;
		Color color2;

		if (i % 2 == 0) {
			color1 = Color(255, 255, 255);
		}
		else {
			color2 = Color(255, 255, 255);
		}

		for (j = 0; j < terrainGridWidth; ++j) {
			int topLeft = i*vertexPerRow + j * 4;
			int topRight = topLeft + 1;
			int bottomRight = topRight + 1;
			int bottomLeft = bottomRight + 1;

			Color color = j % 2 == 0 ? color1 : color2;
			terrainColors[topLeft] = color;
			terrainColors[topRight] = color;
			terrainColors[bottomRight] = color;
			terrainColors[bottomLeft] = color;

			++s;
		}
	}
	
	/*
	for(i = 0; i < squareCount; ++i) {
		Color color = i%4 == 0 ? color1 : color2;
		terrainColors[i*4+0] = color;
		terrainColors[i*4+1] = color;
		terrainColors[i*4+2] = color;
		terrainColors[i*4+3] = color;
	}
	*/

	int resId = m_renderEngine.newResource(vertexCount,vertices,indexCount,indices,terrainColors);
	delete [] vertices;
	delete [] indices;
	delete [] terrainColors;
	return resId;
}