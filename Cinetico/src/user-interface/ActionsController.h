
#ifndef __CINETICO_ACTIONS_CONTROLLER_H__
#define __CINETICO_ACTIONS_CONTROLLER_H__

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
	using namespace cinetico_core;

	class ActionsController : public Controller
	{
	public:

		class GestureItem : public Label {
			ActionsController &m_controller;

			enum State {
				IDLE,
				HOVER,
				PRESSED,
			};

			uilib::Color m_color;
			uilib::Color m_colorSimpleGesture;
			uilib::Color m_colorMovementGesture;

			State m_state;
			bool m_selected;

		public:
			GestureItem(ActionsController &controller);
			void onMouseEnterEvent(MouseEvent &event);
			void onMouseLeaveEvent(MouseEvent &event);
			void onMousePressEvent(MouseEvent &event);
			void onMouseReleaseEvent(MouseEvent &event);

			void setSelected(bool selected);
			void setParam(void *param);

			void update();

			virtual Size getAutoSize();
			bool selected() const { return m_selected; }
		};

	public:
		VerticalLayout layout;
			cPageTitle title;
			HorizontalLayout layoutActionButtons;
				Button buttonAdd;
				Button buttonBack;

			cSeparator sepContent;
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
					HorizontalLayout layoutActionCommands;
						Button buttonAddGesture;
						Button buttonDelGesture;
					HorizontalLayout layoutGestureItems;
					std::vector<GestureItem *> gestureItems;
					VerticalLayout layoutGestureData;
						HorizontalLayout layoutGestureRow1;
							cComboBox cbTransitionType;
							cComboBox cbBodyPoint;
							cComboBox cbRefPoint;
						HorizontalLayout layoutGestureRow2;
							cComboBox cbOperation;
							cTextBox tbValueX;
							cTextBox tbValueY;
							cTextBox tbValueZ;
						//Movement Gesture fields
						HorizontalLayout layoutMovementGestureData;
							cComboBox cbMovementType;
							cTextBox tbMinSpeed;
							cTextBox tbMaxSpeed;
					Button buttonSaveGesture;

		Exercise *m_currentExercise;
		int m_editMode;

		int m_currentActionSelection;
		Action *m_currentAction;
		int m_currentActionTypeSelection;
		int m_currentMovementTypeSelection;
		std::vector<SimpleGesture*> m_gestures;
		int m_selectedGesture;

		void setEditionMode(int mode);
		void updateActionList();

		void fillTransitionTypeCombo(cComboBox &combo);
		void fillBodyPointCombo(cComboBox &combo);
		void fillRefPointCombo(cComboBox &combo);
		void fillOperationCombo(cComboBox &combo);
		void fillMovementTypeCombo(cComboBox &combo);

		void onGestureItemSelected(GestureItem &item);
		GestureItem *addNewGesture(SimpleGesture *gesture);
		void removeAllGestures();
		bool validateCurrentGesture();

		bool validateFields();
		void saveCurrentAction();
		void saveCurrentGesture();

		friend void buttonBack_onClick(Button &button);
		friend void buttonDelete_onClick(Button &button);
		friend void comboActionType_onChange(ComboBox &combo, ComboBoxItem *item);
		friend void buttonDelete_onClick(Button &button);

		//Gestures
		friend void buttonAddGesture_onClick(Button &button);
		friend void buttonDelGesture_onClick(Button &button);
		friend void buttonSaveGesture_onClick(Button &button);

	public:
		friend class GestureRow;
		friend class GestureItem;
		ActionsController(CineticoUI &cineticoUI);

		Layout *viewDefinition();
		virtual void onViewEnter(ViewParams params);
		virtual void onViewUpdate();
		virtual void onViewTick();
		virtual void onViewQuit();
	};

}

#endif