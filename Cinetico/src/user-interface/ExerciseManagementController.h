
#ifndef __CINETICO_EXERCISEMANAGEMENTCONTROLLER_H__
#define __CINETICO_EXERCISEMANAGEMENTCONTROLLER_H__

#include "Controller.h"
#include "uilib/ui/uibase.h"

namespace cinetico {

	using namespace uilib;

	class ExerciseManagementController : public Controller
	{
	public:
		VerticalLayout layout;
			VerticalLayout layoutActions;
				Label labelViewTitle;
				Label labelViewDescr;
				Separator separatorActionButtons;
				HorizontalLayout layoutActionButtons;
					Button buttonAdd;
					Button buttonBack;
				Separator separatorContent;

			VerticalLayout layoutContent;

			HorizontalLayout layoutContentList;
				GridView gridActions;
				VerticalLayout layoutGridActions;
					Button buttonEdit;
					Button buttonDelete;

			VerticalLayout layoutActionData;
				HorizontalLayout layoutActionDataActionButtons;
					Button buttonSaveAction;
					Button buttonCancelAction;

				Separator separatorActionBasicData;
				HorizontalLayout layoutActionDataRow1;
					VerticalLayout layoutActionType;
						Label labelActionType;
						ComboBox comboActionType;
					VerticalLayout layoutPartOf;
						Label labelPartOf;
						ComboBox comboPartOf;
					VerticalLayout layoutName;
						Label labelName;
						EditBox editName;
				HorizontalLayout layoutBaseActionData;
					VerticalLayout layoutBodyPoint;
						Label labelBodyPoint;
						ComboBox comboBodyPoint;
					VerticalLayout layoutRefPoint;
						Label labelRefPoint;
						ComboBox comboRefPoint;
					VerticalLayout layoutMinTime;
						Label labelMinTime;
						EditBox editMinTime;
					VerticalLayout layoutMaxTime;
						Label labelMaxTime;
						EditBox editMaxTime;

				Label labelPosition;
				HorizontalLayout layoutPosition;
					VerticalLayout layoutPositionX;
						Label labelPositionX;
						EditBox editPositionX;
					VerticalLayout layoutPositionY;
						Label labelPositionY;
						EditBox editPositionY;
					VerticalLayout layoutPositionZ;
						Label labelPositionZ;
						EditBox editPositionZ;

				VerticalLayout layoutSpecific;
					Separator separatorSpecificData;
				//Position action
					VerticalLayout layoutPositionSpecific;
						VerticalLayout layoutMinHoldTime;
							Label labelMinHoldtime;
							EditBox editMinHoldtime;

				//Movement action
					VerticalLayout layoutMovementSpecific;
						HorizontalLayout layoutMovementAction;
							VerticalLayout layoutMovementType;
								Label labelMovementType;
								ComboBox comboMovementType;
							VerticalLayout layoutMinSpeed;
								Label labelMinSpeed;
								EditBox editMinSpeed;
							VerticalLayout layoutMaxSpeed;
								Label labelMaxSpeed;
								EditBox editMaxSpeed;

		Exercise *m_currentExercise;
		int m_editMode;

		int m_currentActionSelection;
		Action *m_currentAction;
		int m_currentActionTypeSelection;
		int m_currentMovementTypeSelection;

		void setEditionMode(int mode);
		void updateActionList();

		void fillBodyPointCombo(ComboBox &combo);
		void fillSpaceTypeCombo(ComboBox &combo);
		bool validateFields();
		void saveCurrentAction();

		friend void comboActionType_onChange(ComboBox &combo, ComboBoxItem *item);

	public:
		ExerciseManagementController();

		Layout *viewDefinition();
		void onViewEnter(ViewParams *params);
		void onViewTick();
		void onViewQuit();


	};

}

#endif