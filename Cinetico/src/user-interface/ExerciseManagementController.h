
#ifndef __CINETICO_EXERCISEMANAGEMENTCONTROLLER_H__
#define __CINETICO_EXERCISEMANAGEMENTCONTROLLER_H__

#include "Controller.h"
#include "uilib/ui/uibase.h"
#include "components/PageTitle.h"
#include "components/TextBox.h"
#include "components/ComboBox.h"

namespace cinetico {

	using namespace uilib;
	using cinetico::ComboBox;

	class ExerciseManagementController : public Controller
	{
	public:
		VerticalLayout layout;
			PageTitle title;
			HorizontalLayout layoutActionButtons;
				Button buttonAdd;
				Button buttonBack;

			Separator separatorContent;

			VerticalLayout layoutContent;

			//
				HorizontalLayout layoutContentList;
					GridView gridActions;
					VerticalLayout layoutGridActions;
						Button buttonEdit;
						Button buttonDelete;

			//
				VerticalLayout layoutActionData;
					HorizontalLayout layoutActionDataActionButtons;
						Button buttonSaveAction;
						Button buttonCancelAction;

					Separator separatorActionBasicData;
					HorizontalLayout layoutActionDataRow1;
						ComboBox cbActionType;
						ComboBox cbOrderType;
						TextBox tbName;
					HorizontalLayout layoutBaseActionData;
						ComboBox cbBodyPoint;
						ComboBox cbRefPoint;
						TextBox tbMinTime;
						TextBox tbMaxTime;

					Label labelPosition;
					HorizontalLayout layoutPosition;
						TextBox tbPositionX;
						TextBox tbPositionY;
						TextBox tbPositionZ;

					VerticalLayout layoutSpecific;
						Separator separatorSpecificData;

						//Position action
							VerticalLayout layoutPositionSpecific;
								TextBox tbMinHoldTime;

						//Movement action
							VerticalLayout layoutMovementSpecific;
								HorizontalLayout layoutMovementAction;
									ComboBox cbMovementType;
									TextBox tbMinSpeed;
									TextBox tbMaxSpeed;

		Exercise *m_currentExercise;
		int m_editMode;

		int m_currentActionSelection;
		Action *m_currentAction;
		int m_currentActionTypeSelection;
		int m_currentMovementTypeSelection;

		void setEditionMode(int mode);
		void updateActionList();

		void fillActionTypeCombo(ComboBox &combo);
		void fillOrderTypeCombo(ComboBox &combo);
		void fillBodyPointCombo(ComboBox &combo);
		void fillSpaceTypeCombo(ComboBox &combo);
		void fillMovementTypeCombo(ComboBox &combo);
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