#ifndef __CINETICO_COLLADA_PARSER_H__
#define __CINETICO_COLLADA_PARSER_H__

#include <libxml/tree.h>
#include <vector>
#include "render3d/renderengine.h"

namespace cinetico {

	using namespace render3d;

	class ColladaParser {
		struct ParsingModel {
			std::vector<float> vertices;
			std::vector<float> normals;
			std::vector<float> texCoords;
			std::vector<float> colors;
			std::vector<int> dataIndex;
			int vertexIndex;
			int normalIndex;
			int texCoordIndex;
			int colorIndex;
		};

		RenderEngine &m_engine;
		std::vector<ParsingModel*> m_models;

		xmlNodePtr getFirstChildNode(xmlNodePtr currentNode);
		xmlNodePtr getFirstChildNode(xmlNodePtr currentNode, const char *nodeName);
		xmlNodePtr getNextNode(xmlNodePtr currentNode);
		xmlNodePtr getNextNode(xmlNodePtr currentNode, const char *nodeName);

		void parseIntArray(xmlNodePtr curNode, std::vector<int> &out);
		void parseFloatArray(xmlNodePtr curNode, std::vector<float> &out);

		void readModel(xmlNodePtr rootNode);
		void readVertices(xmlNodePtr meshNode, ParsingModel* model);
		void readNormals(xmlNodePtr meshNode, const char *id, ParsingModel* model);
		void readTextureCoords(xmlNodePtr meshNode, const char *id, ParsingModel* model);
		void readColors(xmlNodePtr meshNode, const char *id, ParsingModel* model);

		ResourceData *assembleModel();

		public:
			ColladaParser(RenderEngine &engine);
			void parse(const char *fileName);
	};

}

#endif