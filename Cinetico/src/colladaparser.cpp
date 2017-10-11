
#include "colladaparser.h"

#include <string>
#include <sstream>
#include <cstring>


#define XMLCHAR(x) ((const xmlChar*)x)
#define XMLCHAR2CHAR(x) ((const char*)x)
#define XMLCHAR_SAFEFREE(x) if(x) xmlFree(x)



namespace cinetico {
	using namespace render3d;
	
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


	void ColladaParser::parseIntArray(xmlNodePtr curNode, std::vector<int> &out) {
		xmlChar *content = curNode->content;

		std::string str = XMLCHAR2CHAR(content);
		int buf; // Have a buffer string
		std::stringstream ss(str); // Insert the string into a stream

		while (ss >> buf)
			out.push_back(buf);
	}

	void ColladaParser::parseFloatArray(xmlNodePtr curNode, std::vector<float> &out) {
		xmlChar *content = curNode->content;

		std::string str = XMLCHAR2CHAR(content);
		float buf;
		std::stringstream ss(str); // Insert the string into a stream

		while (ss >> buf) {
			out.push_back(buf);
		}
	}

	void ColladaParser::readVertices(xmlNodePtr meshNode, ParsingModel* model) {
		if (!meshNode)
			return;
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
										parseFloatArray(child, model->vertices);
									}
								}
							}

							xmlFree(prop2);
							break;
						}

						child = getNextNode(child, "source");
					}
					xmlFree(prop);
				}
			}
		}
	}

	void ColladaParser::readNormals(xmlNodePtr meshNode, const char *id, ParsingModel* model) {
		if (!meshNode)
			return;

		xmlNodePtr child = getFirstChildNode(meshNode, "source");
		while (child) {
			xmlChar *propId = xmlGetProp(child, XMLCHAR("id"));
			if (propId) {
				if (strcmp(id, XMLCHAR2CHAR(propId)) == 0) {
					child = getFirstChildNode(child, "float_array");
					if (child) {
						child = child->children;
						if (child->type == XML_TEXT_NODE) {
							parseFloatArray(child, model->normals);
							xmlFree(propId);
							return;
						}
					}
				}

				xmlFree(propId);
			}

			child = getNextNode(child, "source");
		}
	}

	void ColladaParser::readTextureCoords(xmlNodePtr meshNode, const char *id, ParsingModel* model) {
		if (!meshNode)
			return;

		xmlNodePtr child = getFirstChildNode(meshNode, "source");
		while (child) {
			xmlChar *propId = xmlGetProp(child, XMLCHAR("id"));
			if (propId) {
				if (strcmp(id, XMLCHAR2CHAR(propId)) == 0) {
					child = getFirstChildNode(child, "float_array");
					if (child) {
						child = child->children;
						if (child->type == XML_TEXT_NODE) {
							parseFloatArray(child, model->texCoords);
							xmlFree(propId);
							return;
						}
					}
				}

				xmlFree(propId);
			}

			child = getNextNode(child, "source");
		}
	}

	void ColladaParser::readColors(xmlNodePtr meshNode, const char *id, ParsingModel* model) {
		if (!meshNode)
			return;
		
		xmlNodePtr child = getFirstChildNode(meshNode, "source");
		while (child) {
			xmlChar *propId = xmlGetProp(child, XMLCHAR("id"));
			if (propId) {
				if (strcmp(id, XMLCHAR2CHAR(propId)) == 0) {
					child = getFirstChildNode(child, "float_array");
					if (child) {
						child = child->children;
						if (child->type == XML_TEXT_NODE) {
							parseFloatArray(child, model->colors);
							xmlFree(propId);
							return;
						}
					}
				}

				xmlFree(propId);
			}

			child = getNextNode(child, "source");
		}
	}

	void ColladaParser::readModel(xmlNodePtr rootNode) {
		xmlNodePtr node;

		std::string normalId;
		std::string texCoordId;
		std::string colorId;

		node = getFirstChildNode(rootNode, "library_geometries");
		if (node) {
			node = getFirstChildNode(node, "geometry");
			if (node) {
				do {
					ParsingModel *model = new ParsingModel();
					xmlNodePtr meshNode = getFirstChildNode(node, "mesh");
					if (meshNode) {
						xmlNodePtr triangleChild = getFirstChildNode(meshNode, "triangles");
						if (triangleChild) {
							xmlNodePtr child = getFirstChildNode(triangleChild, "input");
							while (child) {
								xmlChar *prop = xmlGetProp(child, XMLCHAR("semantic"));
								if (prop) {
									xmlChar *propSource = xmlGetProp(child, XMLCHAR("source"));
									if (propSource) {
										if (strcmp(XMLCHAR2CHAR(prop), "NORMAL") == 0) {
											normalId = XMLCHAR2CHAR(&propSource[1]);
										}
										else if (strcmp(XMLCHAR2CHAR(prop), "TEXCOORD")) {
											texCoordId = XMLCHAR2CHAR(&propSource[1]);
										}
										else if (strcmp(XMLCHAR2CHAR(prop), "COLOR") == 0) {
											colorId = XMLCHAR2CHAR(&propSource[1]);
										}

										xmlFree(propSource);
									}

									xmlFree(prop);
								}
								child = getNextNode(child, "input");
							}

							child = getFirstChildNode(triangleChild, "p");
							if (child) {
								child = child->children;
								if (child->type == XML_TEXT_NODE)
									parseIntArray(child, model->dataIndex);
							}
						}

						readVertices(meshNode,model);
						if (*normalId.c_str())
							readNormals(meshNode, normalId.c_str(), model);
						if (*texCoordId.c_str(), model)
							readTextureCoords(meshNode, texCoordId.c_str(), model);
						if (*colorId.c_str())
							readColors(meshNode, colorId.c_str(), model);
					}

					m_models.push_back(model);
				} while (node = getNextNode(node, "geometry"));
			}
		}
	}

	ResourceData *ColladaParser::assembleModel() {
		ParsingModel *model = m_models[2];
	}


	ColladaParser::ColladaParser(RenderEngine &engine)
		: m_engine(engine) {

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

			readModel(rootElement);

			xmlFreeDoc(doc);
		}

		return;
	}
}
