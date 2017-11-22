
#ifndef __CINETICO_ACTIONSCONTROLLER_H__
#define __CINETICO_ACTIONSCONTROLLER_H__

#include "Controller.h"
#include "uilib/ui/uibase.h"
#include "components/cPageTitle.h"
#include "components/cTextBox.h"
#include "components/cComboBox.h"
#include "components/cSeparator.h"

namespace cinetico_core {
	class Exercise;
	class Action;
}

namespace cinetico {

	using namespace uilib;
	using cinetico::ComboBox;
	using namespace cinetico_core;

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
						cTextBox tbName;
						cTextBox tbMinTime;
						cTextBox tbMaxTime;
						cTextBox tbTimeToHold;

					cSeparator separatorGestureData;
					Button buttonAddGesture;
					VerticalLayout layoutGestures;
						HorizontalLayout layoutGestureLabels;
							Label labelTransitionType;
							Label labelBodyPoint;
							Label labelRefPoint;
							Label labelOperation;
							Label labelValueX;
							Label labelValueY;
							Label labelValueZ;
							Button buttonDummy;

		Exercise *m_currentExercise;
		int m_editMode;

		int m_currentActionSelection;
		Action *m_currentAction;
		int m_currentActionTypeSelection;
		int m_currentMovementTypeSelection;
		std::vector<SimpleGesture*> m_gestures;

		void setEditionMode(int mode);
		void updateActionList();

		void fillTransitionTypeCombo(cComboBox &combo);
		void fillBodyPointCombo(cComboBox &combo);
		void fillRefPointCombo(cComboBox &combo);
		void fillOperationCombo(cComboBox &combo);
		void fillMovementTypeCombo(cComboBox &combo);
		void removeAllGestures();
		bool validateFields();
		void saveCurrentAction();

		friend void buttonBack_onClick(Button &button);
		friend void buttonDelete_onClick(Button &button);
		friend void comboActionType_onChange(ComboBox &combo, ComboBoxItem *item);
		friend void buttonDelete_onClick(Button &button);
		friend void buttonAddGesture_onClick(Button &button);

	public:
		friend class GestureRow;
		ActionsController(CineticoUI &cineticoUI);

		Layout *viewDefinition();
		virtual void onViewEnter(ViewParams params);
		virtual void onViewUpdate();
		virtual void onViewTick();
		virtual void onViewQuit();
	};

}

#endif