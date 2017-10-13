#ifndef __CINETICO_DICTIONARY_H__
#define __CINETICO_DICTIONARY_H__

#include <vector>
#include <string>

namespace cinetico {

	class Cinetico;

	class Dictionary {
	public:
		enum LanguageID {
			PT_BR,
			EN_US,

			LanguageIDCount,
			InvalidLanguageID = -1
		};

		enum StringID {
			//
			ActionLogoff,

			DefaultActionAdd,
			DefaultActionEdit,
			DefaultActionDelete,
			DefaultActionSave,
			DefaultActionCancel,
			DefaultActionYes,
			DefaultActionNo,

			DefaultActionQuestionDelete,

			//Entity
			EntityUserProfile,
			EntityExercise,
			EntityAction,

			//Entitiy attributes
			ExerciseName,
			ExerciseAuthor,
			ExercisePublic,

			//Views

			ViewDefaultErrorRequiredFields,

			//Login
			LoginViewTitle,
			LoginViewDesc,
			LoginViewSectionLogin,
			LoginViewSectionCreateUserProfile,
			LoginViewUsername,
			LoginViewPassword,
			LoginViewConfirmPassword,
			LoginViewActionLogin,
			LoginViewActionCreateUserProfile,

			LoginViewErrorUserProfileCreatedSuccessfully,
			LoginViewErrorUserLoggedSucessfully,
			LoginViewErrorInvalidCredentials,
			LoginViewErrorUsernameAlreadyExists,

			//Exercises
			ExercisesViewTitle,
			ExercisesViewDesc,
			ExercisesViewActionCreate,
			ExercisesViewActionManageActions,
			ExercisesViewActionDoExercise,
			ExercisesViewGridColumnActionCount,
			ExercisesViewExerciseName,
			ExercisesViewSectionBodyPoints,

			ExercisesViewErrorExerciseAlreadyExists,
			ExercisesViewErrorExerciseHasNoActions,

			//Action
			ActionsViewTitle,
			ActionsViewDesc,
			UserProfileViewTitle,
			UserProfileViewDesc,

			StringIDCount,
			InvalidStringID = -1
		};

	private:
		void __fillMaps();
		LanguageID xmlIDAttrToLanguageID(const char *attr);
		StringID xmlIDAttrToStringID(const char *attr);
		void loadLanguage(const char *filename);
		void loadAllLanguages();

		Cinetico &m_cinetico;
		std::vector<std::vector<std::string>> m_languages;

		LanguageID m_languageId;

	public:
		Dictionary(Cinetico &cinetico);

		bool setLanguage(LanguageID id);

		std::string getString(StringID id, ...);
	};

}

#endif