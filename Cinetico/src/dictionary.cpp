
#include "cinetico.h"
#include "dictionary.h"
#include <libxml/tree.h>
#include "uilib/lib/string.h"

#include <windows.h> //for utf-8 to ansi conversion (temporary) todo: remove when it is not needed anymore

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


			//String mapping
			//General Data
			MAP_STR(X);
			MAP_STR(Y);
			MAP_STR(Z);

			//Body Points
			MAP_STR(BodyPointHead);
			MAP_STR(BodyPointCervical);
			MAP_STR(BodyPointSpine);
			MAP_STR(BodyPointSpineBase);
			MAP_STR(BodyPointLeftShoulder);
			MAP_STR(BodyPointLeftElbow);
			MAP_STR(BodyPointLeftWrist);
			MAP_STR(BodyPointLeftPalm);
			MAP_STR(BodyPointLeftPinky);
			MAP_STR(BodyPointLeftRingFinger);
			MAP_STR(BodyPointLeftMiddleFinger);
			MAP_STR(BodyPointLeftIndexFinger);
			MAP_STR(BodyPointLeftThumb);
			MAP_STR(BodyPointLeftHip);
			MAP_STR(BodyPointLeftKnee);
			MAP_STR(BodyPointLeftAnkle);
			MAP_STR(BodyPointLeftFoot);
			MAP_STR(BodyPointRightShoulder);
			MAP_STR(BodyPointRightElbow);
			MAP_STR(BodyPointRightWrist);
			MAP_STR(BodyPointRightPalm);
			MAP_STR(BodyPointRightPinky);
			MAP_STR(BodyPointRightRingFinger);
			MAP_STR(BodyPointRightMiddleFinger);
			MAP_STR(BodyPointRightIndexFinger);
			MAP_STR(BodyPointRightThumb);
			MAP_STR(BodyPointRightHip);
			MAP_STR(BodyPointRightKnee);
			MAP_STR(BodyPointRightAnkle);
			MAP_STR(BodyPointRightFoot);

			//Action Enums
			MAP_STR(ActionTypePosition);
			MAP_STR(ActionTypeMovement);

			MAP_STR(ActionRefPointAny);
			MAP_STR(ActionRefPointLastPosition);

			MAP_STR(MovementTypeLinear);
			MAP_STR(MovementTypeAngular);

			//
			MAP_STR(ActionLogoff);

			MAP_STR(DefaultActionAdd);
			MAP_STR(DefaultActionEdit);
			MAP_STR(DefaultActionDelete);
			MAP_STR(DefaultActionSave);
			MAP_STR(DefaultActionCancel);
			MAP_STR(DefaultActionYes);
			MAP_STR(DefaultActionNo);
			MAP_STR(DefaultActionBack);

			MAP_STR(DefaultActionQuestionDelete);

			//Entity
			MAP_STR(EntityUserProfile);
			MAP_STR(EntityExercise);
			MAP_STR(EntityAction);

			//Entitiy attributes
			MAP_STR(UserProfileLoginName);
			MAP_STR(UserProfileUserName);
			MAP_STR(UserProfilePassword);
			MAP_STR(UserProfileCreationDate);

			MAP_STR(ExerciseName);
			MAP_STR(ExerciseAuthor);
			MAP_STR(ExercisePublic);

			MAP_STR(ActionType);
			MAP_STR(ActionOrder);
			MAP_STR(ActionName);
			MAP_STR(ActionMinTime);
			MAP_STR(ActionMaxTime);
			MAP_STR(ActionTimeToHold);
			MAP_STR(ActionBodyPoint);
			MAP_STR(ActionRefPointX);
			MAP_STR(ActionRefPointY);
			MAP_STR(ActionRefPointZ);
			MAP_STR(ActionFinalPosition);
			MAP_STR(ActionFinalOrientation);

			MAP_STR(PositionActionMinHoldTime);
			MAP_STR(MovementActionType);
			MAP_STR(MovementActionMinSpeed);
			MAP_STR(MovementActionMaxSpeed);

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

			//UserProfile
			MAP_STR(UserProfileViewTitle);
			MAP_STR(UserProfileViewDesc);
			MAP_STR(UserProfileViewSectionUserDetails);
			MAP_STR(UserProfileViewSectionChangePassword);
			MAP_STR(UserProfileViewCurrentPassword);
			MAP_STR(UserProfileViewNewPassword);
			MAP_STR(UserProfileViewNewPasswordConfirmation);
			MAP_STR(UserProfileViewSectionDeactivateUser);
			MAP_STR(UserProfileViewDeactivateUser);

			MAP_STR(UserProfileViewErrorUserNameChangedSucessfully);
			MAP_STR(UserProfileViewErrorPasswordChangedSucessfully);
			MAP_STR(UserProfileViewErrorPasswordWrongCurrentPassword);
			MAP_STR(UserProfileViewErrorPasswordConfirmationNotEqual);
			MAP_STR(UserProfileViewQuestionDeactivateConfirmation);

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

			//Actions
			MAP_STR(ActionsViewTitle);
			MAP_STR(ActionsViewDesc);
			MAP_STR(ActionsViewSectionBasicData);
			MAP_STR(ActionsViewSectionSpecificData);

			g_mapsFilled = true;
		}
	}

	Dictionary::LanguageID Dictionary::xmlIDAttrToLanguageID(const char *attr) {
		LanguageID langId = InvalidLanguageID;
		uilib::string langIdStr = attr;
		langIdStr = langIdStr.toUpper();
		for (unsigned i = 0; i < LanguageIDCount; ++i) {
			if (langIdStr == g_langIdStr[i]) {
				langId = (LanguageID)i;
				break;
			}
		}

		return langId;
	}

	Dictionary::StringID Dictionary::xmlIDAttrToStringID(const char *attr) {
		StringID stringId = InvalidStringID;
		uilib::string stringIdStr = attr;
		for (unsigned int i = 0; i < StringIDCount; ++i) {
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
							const char *utf8 = XMLCHAR2CHAR(text->content);
							WCHAR *utf16;
							unsigned int sizeNeeded;
							sizeNeeded = ::MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
							utf16 = new WCHAR[sizeNeeded];
							::MultiByteToWideChar(CP_UTF8, 0, utf8, -1, utf16, sizeNeeded);

							char *ansi;
							sizeNeeded = ::WideCharToMultiByte(CP_ACP, 0, utf16, -1, NULL, 0, NULL, NULL);
							ansi = new char[sizeNeeded];
							::WideCharToMultiByte(CP_ACP, 0, utf16, -1, ansi, sizeNeeded, NULL, NULL);
							m_languages[langId][stringId] = ansi;
							delete[] ansi;
							delete[] utf16;
						}
					}
				}

			}

			node = node->next;
		}

		xmlFreeDoc(doc);
	}

	void Dictionary::loadAllLanguages() {
		loadLanguage("languages/pt-br.clang");
		loadLanguage("languages/en-us.clang");
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

	uilib::string Dictionary::getString(StringID id, ...) {
		if (id >= StringIDCount)
			return "";

		uilib::string fmtStr = m_languages[m_languageId][id];

		const char *s = fmtStr.data();
		const char *c = s;

		va_list ap;
		va_start(ap, id);

		while (*c) {
			if (*c == '%') {
				char argType = *(c + 1);
				int index = c - s;
				if (argType == 's') {
					const char *aStr = va_arg(ap, const char*);
					fmtStr.replace(index, 2, aStr);
				}
				else if (argType == 'd') {
					int aInt = va_arg(ap, int);
					fmtStr.replace(index, 2, uilib::string::fromInteger(aInt));
				}
				else if (argType == 'f') {
					float aFloat = va_arg(ap, float);
					fmtStr.replace(index, 2, uilib::string::fromFloat(aFloat));
				}
				else if (argType == '%') {
					fmtStr.replace(index, 2, "%");
				}
				else {
					continue;
				}
				c += 2;
			}
			++c;
		}
		va_end(ap);

		return fmtStr;
	}

}