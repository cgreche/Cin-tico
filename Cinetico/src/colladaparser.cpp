
#include "colladaparser.h"

#include <cstring>

#define XMLCHAR(x) ((const xmlChar*)x)
#define XMLCHAR2CHAR(x) ((const char*)x)
#define XMLCHAR_SAFEFREE(x) if(x) xmlFree(x)

namespace cinetico {
	
	xmlNodePtr ColladaParser::getFirstChildNode(xmlNodePtr currentNode)
	{
		if (currentNode && currentNode->type == XML_ELEMENT_NODE) {
			xmlNodePtr node = currentNode->children;
			if (node && node->type != XML_ELEMENT_NODE)
				node = getNextNode(node);
			return node;
		}

		return NULL;
	}

	xmlNodePtr ColladaParser::getNextNode(xmlNodePtr currentNode)
	{
		if (currentNode) {
			xmlNodePtr node = currentNode->next;
			while (node != NULL) {
				if (node->type == XML_ELEMENT_NODE)
					return node;

				node = node->next;
			}
		}
		return NULL;
	}

	xmlNodePtr ColladaParser::getFirstChildNode(xmlNodePtr currentNode, const char *nodeName)
	{
		if (currentNode) {
			xmlNodePtr node = getFirstChildNode(currentNode);
			if (node && nodeName) {
				do {
					if (strcmp(XMLCHAR2CHAR(node->name), nodeName) == 0)
						return node;
					node = getNextNode(node);
				} while (node);
			}
			return node;
		}

		return NULL;
	}

	xmlNodePtr ColladaParser::getNextNode(xmlNodePtr currentNode, const char *nodeName)
	{
		if (currentNode) {
			xmlNodePtr node = getNextNode(currentNode);
			if (node) {
				do {
					if (strcmp(XMLCHAR2CHAR(node->name), nodeName) == 0)
						return node;
					node = getNextNode(node);
				} while (node);
			}
		}
		return NULL;
	}

	void ColladaParser::parseFloat3Array(xmlNodePtr curNode) {
		xmlChar *content = curNode->content;
	}


	void ColladaParser::readGeometry(xmlNodePtr rootNode) {
		xmlNodePtr node;

		node = getFirstChildNode(rootNode,"library_geometries");
		if (node) {
			node = getFirstChildNode(node, "geometry");
			do {
				xmlNodePtr meshNode = getFirstChildNode(node, "mesh");
				if (meshNode) {
					xmlNodePtr child = getFirstChildNode(meshNode, "vertices");
					if (child) {
						child = getFirstChildNode(child, "input");
						if (child) {
							xmlChar *prop = xmlGetProp(child, XMLCHAR("source"));
							if (prop) {
								child = getFirstChildNode(meshNode, "source");
								while (child) {
									xmlChar *prop2 = xmlGetProp(child, XMLCHAR("id"));
									if (prop2) {
										if (strcmp(XMLCHAR2CHAR(&prop[1]), XMLCHAR2CHAR(prop2)) == 0) {
											child = getFirstChildNode(child, "float_array");
											if (child) {
												child = child->children;
												if (child->type == XML_TEXT_NODE) {
													parseFloat3Array();
												}
											}
										}

										xmlFree(prop2);
										break;
									}

									child = getNextNode(node, "source");
								}
								xmlFree(prop);
							}
						}
					}
				}
			} while (node = getNextNode(node,"geometry"));
		}

	}

	void ColladaParser::parse(const char *fileName)
	{
		//init
		xmlDocPtr doc = NULL;
		xmlNodePtr rootElement = NULL;

		doc = xmlReadFile(fileName, NULL, 0);
		if (doc == NULL) {
			//	printf("Error: could not parse file %s\n", fullXMLFilePath);
			return;
		}

		rootElement = xmlDocGetRootElement(doc);
		if (rootElement == NULL) {
			printf("%s File is empty.\n", fileName);
		}
		else {
			if (rootElement->type != XML_ELEMENT_NODE || xmlStrcmp(rootElement->name, XMLCHAR("COLLADA")) != 0) {
				//invalid file
				return;
			}

			readGeometry(rootElement);

			xmlFreeDoc(doc);
		}

		return;
	}
}
