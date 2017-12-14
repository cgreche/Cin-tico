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
			__StringIDStart = -1,

			AppTitle

			//General Data
			, X
			, Y
			, Z

			//Body Points
			, BodyPointHead
			, BodyPointNeck
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

			//Exercise enums
			, ExerciseStateIdle
			, ExerciseStateRunning
			, ExerciseStateFinished

			//Gesture enums
			, SimpleGestureTransitionTypeFree
			, SimpleGestureTransitionTypeFixedMovement

			, SimpleGestureRefPointAny
			, SimpleGestureRefPointLastPosition

			, SimpleGestureOperationFixedPosition
			, SimpleGestureOperationInFront
			, SimpleGestureOperationBehind
			, SimpleGestureOperationToRight
			, SimpleGestureOperationToLeft
			, SimpleGestureOperationAbove
			, SimpleGestureOperationBelow
			, SimpleGestureOperationAtSameBreadth
			, SimpleGestureOperationAtSameHeight
			, SimpleGestureOperationAtSameDepth
			, SimpleGestureOperationFixedOrientation
			, SimpleGestureOperationOrientationLookingTo

			, MovementGestureMovementTypeLinear
			, MovementGestureMovementTypeSmooth

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
			, EntityGesture
			, EntityGeneralSettings

			//Entitiy attributes
			, UserProfileLoginName
			, UserProfileUserName
			, UserProfilePassword
			, UserProfileCreationDate

			, ExerciseName
			, ExerciseAuthor
			, ExercisePublic

			, ActionName
			, ActionMinExecutionTime
			, ActionMaxExecutionTime
			, ActionTimeToHold
			, ActionGestureCount

			, SimpleGestureTransitionType
			, SimpleGestureBodyPoint
			, SimpleGestureRefPoint
			, SimpleGestureOperation
			, SimpleGestureValueX
			, SimpleGestureValueY
			, SimpleGestureValueZ

			, MovementGestureMovementType
			, MovementGestureMinSpeed
			, MovementGestureMaxSpeed

			//General Config
			, GeneralSettingsCorePosDistThreshold
			, GeneralSettingsCorePosMinHoldtime
			, GeneralSettingsAdapter
			, GeneralSettingsResolution
			, GeneralSettingsFullscreen
			, GeneralSettingsAntialiasing

			//Views
			, ViewDefaultErrorRequiredFields

			//MainWindow
			, MainWindowUsername

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
			, ActionsViewSectionGestures
			, ActionsViewErrorNoGestures
			, ActionsViewErrorInvalidGestures

			//PlayMode
			, PlayModeViewTitle
			, PlayModeExerciseRealizationSelectedExercise
			, PlayModeExerciseState

			//General Settings
			, GeneralSettingsViewTitle
			, GeneralSettingsViewDesc
			, GeneralSettingsViewSectionCore
			, GeneralSettingsViewSectionGraphics
			, GeneralSettingsViewTestSensor
			, GeneralSettingsViewTestSensorErrorSuccess
			, GeneralSettingsViewTestSensorErrorNotDetected

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