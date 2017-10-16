
#ifndef __CINETICO_EXERCISEMANAGEMENTCONTROLLER_H__
#define __CINETICO_EXERCISEMANAGEMENTCONTROLLER_H__

#include "Controller.h"
#include "uilib/ui/uibase.h"
#include "components/cPageTitle.h"
#include "components/cTextBox.h"
#include "components/cComboBox.h"
#include "components/cSeparator.h"

namespace cinetico {

	using namespace uilib;
	using cinetico::ComboBox;

	class ActionsController : public Controller
	{
	public:
		VerticalLayout layout;
			cPageTitle title;
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

					cSeparator separatorActionBasicData;
					HorizontalLayout layoutActionDataRow1;
						cComboBox cbActionType;
						cComboBox cbOrderType;
						cTextBox tbName;
					HorizontalLayout layoutBaseActionData;
						cComboBox cbBodyPoint;
						cComboBox cbRefPoint;
						cTextBox tbMinTime;
						cTextBox tbMaxTime;

					Label labelPosition;
					HorizontalLayout layoutPosition;
						cTextBox tbPositionX;
						cTextBox tbPositionY;
						cTextBox tbPositionZ;

					VerticalLayout layoutSpecific;
						cSeparator separatorSpecificData;

						//Position action
						VerticalLayout layoutPositionSpecific;
							cTextBox tbMinHoldTime;

						//Movement action
						VerticalLayout layoutMovementSpecific;
							HorizontalLayout layoutMovementAction;
								cComboBox cbMovementType;
								cTextBox tbMinSpeed;
								cTextBox tbMaxSpeed;

		Exercise *m_currentExercise;
		int m_editMode;

		int m_currentActionSelection;
		Action *m_currentAction;
		int m_currentActionTypeSelection;
		int m_currentMovementTypeSelection;

		void setEditionMode(int mode);
		void updateActionList();

		void fillActionTypeCombo(cComboBox &combo);
		void fillOrderTypeCombo(cComboBox &combo);
		void fillBodyPointCombo(cComboBox &combo);
		void fillSpaceTypeCombo(cComboBox &combo);
		void fillMovementTypeCombo(cComboBox &combo);
		bool validateFields();
		void saveCurrentAction();

		friend void comboActionType_onChange(ComboBox &combo, ComboBoxItem *item);
		friend void buttonDelete_onClick(Button &button);

	public:
		ActionsController(Cinetico &cinetico);

		Layout *viewDefinition();
		virtual void onViewEnter(ViewParams *params);
		virtual void onViewUpdate();
		virtual void onViewTick();
		virtual void onViewQuit();
	};

}

#endif