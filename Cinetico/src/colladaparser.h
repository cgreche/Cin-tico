#ifndef __CINETICO_COLLADA_PARSER_H__
#define __CINETICO_COLLADA_PARSER_H__

#include <libxml/tree.h>

namespace cinetico {

	class ColladaParser {

		xmlNodePtr getFirstChildNode(xmlNodePtr currentNode);
		xmlNodePtr getFirstChildNode(xmlNodePtr currentNode, const char *nodeName);
		xmlNodePtr getNextNode(xmlNodePtr currentNode);
		xmlNodePtr getNextNode(xmlNodePtr currentNode, const char *nodeName);


		void readGeometry(xmlNodePtr rootNode);

		public:
			void parse(const char *fileName);
	};

}

#endif