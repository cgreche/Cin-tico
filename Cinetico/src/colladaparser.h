#ifndef __CINETICO_COLLADA_PARSER_H__
#define __CINETICO_COLLADA_PARSER_H__

#include <libxml/tree.h>

namespace cinetico {

	using namespace render3d;

	class ColladaParser {

		std::vector<Vector3> m_vertices;

		xmlNodePtr getFirstChildNode(xmlNodePtr currentNode);
		xmlNodePtr getFirstChildNode(xmlNodePtr currentNode, const char *nodeName);
		xmlNodePtr getNextNode(xmlNodePtr currentNode);
		xmlNodePtr getNextNode(xmlNodePtr currentNode, const char *nodeName);

		void parseIntArray(std::vector<int> &out);
		void parseFloat3Array(std::vector<Vector3>& out);



		void readGeometry(xmlNodePtr rootNode);

		public:
			void parse(const char *fileName);
	};

}

#endif