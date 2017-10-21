#ifndef __CINETICO_DICTIONARY_H__
#define __CINETICO_DICTIONARY_H__

#include <vector>
#include <string>

#include "uilib/lib/string.h"

namespace cinetico {

	class Cinetico;

	class Dictionary {
	public:
		enum LanguageID {
			PT_BR,
			EN_US,
			ES_ES,

			LanguageIDCount,
			InvalidLanguageID = -1
		};

		enum StringID {
			//General Data
			  X
			, Y
			, Z

			//Body Points
			, BodyPointHead
			, BodyPointCervical
			, BodyPointSpine
			, BodyPointSpineBase
			, BodyPointLeftShoulder
			, BodyPointLeftElbow
			, BodyPointLeftWrist
			, BodyPointLeftPalm
			, BodyPointLeftPinky
			, BodyPointLeftRingFinger
			, BodyPointLeftMiddleFinger
			, BodyPointLeftIndexFinger
			, BodyPointLeftThumb
			, BodyPointLeftHip
			, BodyPointLeftKnee
			, BodyPointLeftAnkle
			, BodyPointLeftFoot
			, BodyPointRightShoulder
			, BodyPointRightElbow
			, BodyPointRightWrist
			, BodyPointRightPalm
			, BodyPointRightPinky
			, BodyPointRightRingFinger
			, BodyPointRightMiddleFinger
			, BodyPointRightIndexFinger
			, BodyPointRightThumb
			, BodyPointRightHip
			, BodyPointRightKnee
			, BodyPointRightAnkle
			, BodyPointRightFoot

			//Action Enums
			, ActionTypePosition
			, ActionTypeMovement

			, ActionRefPointAny
			, ActionRefPointLastPosition

			, MovementTypeLinear
			, MovementTypeAngular

			//
			, ActionLogoff

			, DefaultActionAdd
			, DefaultActionEdit
			, DefaultActionDelete
			, DefaultActionSave
			, DefaultActionCancel
			, DefaultActionYes
			, DefaultActionNo
			, DefaultActionBack

			, DefaultActionQuestionDelete

			//Entity
			, EntityUserProfile
			, EntityExercise
			, EntityAction

			//Entitiy attributes
			, UserProfileLoginName
			, UserProfileUserName
			, UserProfilePassword
			, UserProfileCreationDate

			, ExerciseName
			, ExerciseAuthor
			, ExercisePublic

			, ActionType
			, ActionOrder
			, ActionName
			, ActionMinTime
			, ActionMaxTime
			, ActionBodyPoint
			, ActionRefPointX
			, ActionRefPointY
			, ActionRefPointZ
			, ActionFinalPosition
			, ActionFinalOrientation

			, PositionActionMinHoldTime
			, MovementActionType
			, MovementActionMinSpeed
			, MovementActionMaxSpeed

			//Views
			, ViewDefaultErrorRequiredFields

			//Login
			, LoginViewTitle
			, LoginViewDesc
			, LoginViewSectionLogin
			, LoginViewSectionCreateUserProfile
			, LoginViewUsername
			, LoginViewPassword
			, LoginViewConfirmPassword
			, LoginViewActionLogin
			, LoginViewActionCreateUserProfile

			, LoginViewErrorUserProfileCreatedSuccessfully
			, LoginViewErrorUserLoggedSucessfully
			, LoginViewErrorInvalidCredentials
			, LoginViewErrorUsernameAlreadyExists

			//UserProfile
			, UserProfileViewTitle
			, UserProfileViewDesc
			, UserProfileViewSectionUserDetails
			, UserProfileViewSectionChangePassword
			, UserProfileViewCurrentPassword
			, UserProfileViewNewPassword
			, UserProfileViewNewPasswordConfirmation
			, UserProfileViewSectionDeactivateUser
			, UserProfileViewDeactivateUser

			, UserProfileViewErrorUserNameChangedSucessfully
			, UserProfileViewErrorPasswordChangedSucessfully
			, UserProfileViewErrorPasswordWrongCurrentPassword
			, UserProfileViewErrorPasswordConfirmationNotEqual
			, UserProfileViewQuestionDeactivateConfirmation


			//Exercises
			, ExercisesViewTitle
			, ExercisesViewDesc
			, ExercisesViewActionCreate
			, ExercisesViewActionManageActions
			, ExercisesViewActionDoExercise
			, ExercisesViewGridColumnActionCount
			, ExercisesViewExerciseName
			, ExercisesViewSectionBodyPoints

			, ExercisesViewErrorExerciseAlreadyExists
			, ExercisesViewErrorExerciseHasNoActions

			//Action
			, ActionsViewTitle
			, ActionsViewDesc
			, ActionsViewSectionBasicData
			, ActionsViewSectionSpecificData

			//
			, StringIDCount
			, InvalidStringID = -1
		};

	private:
		void __fillMaps();
		LanguageID xmlIDAttrToLanguageID(const char *attr);
		StringID xmlIDAttrToStringID(const char *attr);
		void loadLanguage(const char *filename);
		void loadAllLanguages();

		Cinetico &m_cinetico;
		std::vector<std::vector<uilib::string>> m_languages;

		LanguageID m_languageId;

	public:
		Dictionary(Cinetico &cinetico);

		bool setLanguage(LanguageID id);

		uilib::string getString(StringID id, ...);
	};

}

#endif