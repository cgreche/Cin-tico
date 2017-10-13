
#include "cinetico.h"
#include "dictionary.h"
#include <libxml/tree.h>

#define XMLCHAR(x) ((const xmlChar*)x)
#define XMLCHAR2CHAR(x) ((const char*)x)
#define XMLCHAR_SAFEFREE(x) if(x) xmlFree(x)

namespace cinetico {

	static bool g_mapsFilled = false;

	const char *g_langIdStr[Dictionary::LanguageIDCount];
	const char *g_stringIdStr[Dictionary::StringIDCount];

#define MAP_LANG(id) g_langIdStr[id] = #id
#define MAP_STR(id) g_stringIdStr[id] = #id

	void Dictionary::__fillMaps() {
		if (!g_mapsFilled) {
			MAP_LANG(PT_BR);
			MAP_LANG(EN_US);

			//

			MAP_STR(ActionLogoff);

			//Default actions
			MAP_STR(DefaultActionAdd);
			MAP_STR(DefaultActionEdit);
			MAP_STR(DefaultActionDelete);
			MAP_STR(DefaultActionSave);
			MAP_STR(DefaultActionCancel);
			MAP_STR(DefaultActionYes);
			MAP_STR(DefaultActionNo);

			MAP_STR(DefaultActionQuestionDelete);

			//Entity
			MAP_STR(EntityUserProfile);
			MAP_STR(EntityExercise);
			MAP_STR(EntityAction);

			//Entitiy attributes
			MAP_STR(ExerciseName);
			MAP_STR(ExerciseAuthor);
			MAP_STR(ExercisePublic);
			
			//Views
			MAP_STR(ViewDefaultErrorRequiredFields);

			//Login
			MAP_STR(LoginViewTitle);
			MAP_STR(LoginViewDesc);
			MAP_STR(LoginViewSectionLogin);
			MAP_STR(LoginViewSectionCreateUserProfile);
			MAP_STR(LoginViewUsername);
			MAP_STR(LoginViewPassword);
			MAP_STR(LoginViewConfirmPassword);
			MAP_STR(LoginViewActionLogin);
			MAP_STR(LoginViewActionCreateUserProfile);

			MAP_STR(LoginViewErrorUserProfileCreatedSuccessfully);
			MAP_STR(LoginViewErrorUserLoggedSucessfully);
			MAP_STR(LoginViewErrorInvalidCredentials);
			MAP_STR(LoginViewErrorUsernameAlreadyExists);

			//Exercises
			MAP_STR(ExercisesViewTitle);
			MAP_STR(ExercisesViewDesc);
			MAP_STR(ExercisesViewActionCreate);
			MAP_STR(ExercisesViewActionManageActions);
			MAP_STR(ExercisesViewActionDoExercise);
			MAP_STR(ExercisesViewGridColumnActionCount);
			MAP_STR(ExercisesViewExerciseName);
			MAP_STR(ExercisesViewSectionBodyPoints);

			MAP_STR(ExercisesViewErrorExerciseAlreadyExists);
			MAP_STR(ExercisesViewErrorExerciseHasNoActions);

			//Action
			MAP_STR(ActionsViewTitle);
			MAP_STR(ActionsViewDesc);
			MAP_STR(UserProfileViewTitle);
			MAP_STR(UserProfileViewDesc);

			g_mapsFilled = true;
		}
	}

	Dictionary::LanguageID Dictionary::xmlIDAttrToLanguageID(const char *attr) {
		LanguageID langId = InvalidLanguageID;
		std::string langIdStr = attr;
		unsigned int i;

		//toLower()
		for (i = 0; i < langIdStr.length(); ++i)
			langIdStr[i] = toupper(langIdStr[i]);

		for (i = 0; i < LanguageIDCount; ++i) {
			if (langIdStr == g_langIdStr[i]) {
				langId = (LanguageID)i;
				break;
			}
		}

		return langId;
	}

	Dictionary::StringID Dictionary::xmlIDAttrToStringID(const char *attr) {
		StringID stringId = InvalidStringID;
		std::string stringIdStr = attr;
		unsigned int i;
		for (i = 0; i < StringIDCount; ++i) {
			if (stringIdStr == g_stringIdStr[i]) {
				stringId = (StringID)i;
				break;
			}
		}

		return stringId;
	}

	void Dictionary::loadLanguage(const char *filename) {
		//init
		xmlDocPtr doc = NULL;
		xmlNodePtr rootElement = NULL;

		doc = xmlReadFile(filename, NULL, 0);
		if (doc == NULL) {
			return;
		}

		rootElement = xmlDocGetRootElement(doc);
		if (rootElement == NULL) {
			return;
		}

		if (rootElement->type != XML_ELEMENT_NODE || xmlStrcmp(rootElement->name, XMLCHAR("language")) != 0) {
			return;
		}

		xmlChar * xmlLangIdAttr = xmlGetProp(rootElement, XMLCHAR("id"));
		if (!xmlLangIdAttr)
			return;

		LanguageID langId = xmlIDAttrToLanguageID(XMLCHAR2CHAR(xmlLangIdAttr));
		xmlFree(xmlLangIdAttr);
		if (langId == InvalidLanguageID) {
			return;
		}

		xmlNodePtr node = rootElement->children;
		while (node) {
			if (node->type == XML_ELEMENT_NODE && xmlStrcmp(node->name, XMLCHAR("string")) == 0) {
				xmlChar * xmlStringIdAttr = xmlGetProp(node, XMLCHAR("id"));
				if (xmlStringIdAttr) {
					StringID stringId = xmlIDAttrToStringID(XMLCHAR2CHAR(xmlStringIdAttr));
					xmlFree(xmlStringIdAttr);
					if (stringId != InvalidStringID) {
						xmlNodePtr text = node->children;
						if (text && text->type == XML_TEXT_NODE) {
							m_languages[langId][stringId] = XMLCHAR2CHAR(text->content);
						}
					}
				}

			}

			node = node->next;
		}

		xmlFreeDoc(doc);
	}

	void Dictionary::loadAllLanguages() {
		loadLanguage("language/pt-br.clang");
		loadLanguage("language/en-us.clang");
	}

	Dictionary::Dictionary(Cinetico &cinetico)
		: m_cinetico(cinetico) {
		m_languages.resize(LanguageIDCount);
		for (unsigned int i = 0; i < m_languages.size(); ++i)
			m_languages[i].resize(StringIDCount);
		__fillMaps();
		loadAllLanguages();
		m_languageId = PT_BR;
	}

	bool Dictionary::setLanguage(LanguageID id) {
		if (id >= LanguageIDCount)
			return false;

		m_languageId = id;
		return true;
	}

	std::string Dictionary::getString(StringID id, ...) {
		if (id >= StringIDCount)
			return "";

		/*
		va_list ap;
		va_start(ap, id);
		int max = va_arg(ap, int);

		va_end(ap);
		*/

		return m_languages[m_languageId][id];
	}

}