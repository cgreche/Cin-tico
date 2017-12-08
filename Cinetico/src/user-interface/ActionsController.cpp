
#include "cinetico.h"
#include "cineticodb.h"
#include "cineticoui.h"

#include "core/entity/Action.h"

#include "ActionsController.h"

#include <sstream>

namespace cinetico {
	using namespace cinetico_core;
	using cinetico::ComboBox;

	void Gesture_buttonDelete_onClick(Button &button) {
		ActionsController::GestureRow *gestureRow = (ActionsController::GestureRow*)button.param();
		gestureRow->removeSelf();
	}

	/*
	class GestureItem : public Label {
	public:
		void onMouseEnterEvent(MouseEvent &event) {
			setBackgroundColor(uilib::Color(0, 200, 200));
		}

		void onMouseLeaveEvent(MouseEvent &event) {
			setBackgroundColor(uilib::Color(200, 20, 20));
		}
	};

	GestureItem gestureItem[20];

	for (int i = 0; i < 20; ++i) {
		gestureItem[i].setTransparent(false);
		gestureItem[i].setBackgroundColor(uilib::Color(200, 20, 20));
		gestureItem[i].setText(string::fromInteger(i));
		gestureItem[i].setAlignment(Label::VCenter | Label::HCenter);
		gestureItem[i].setTextColor(uilib::Color(255, 255, 255));
		layoutGestures.append(gestureItem[i], Size(16, 34));
	}
	testLayout.append(layoutGestures);
	*/
	ActionsController::GestureRow::GestureRow(ActionsController &controller)
		: m_controller(controller) {
		append(cbTransitionType);
		controller.fillTransitionTypeCombo(cbTransitionType);
		append(cbBodyPoint);
		controller.fillBodyPointCombo(cbBodyPoint);
		append(cbRefPoint);
		controller.fillRefPointCombo(cbRefPoint);
		append(cbOperation);
		controller.fillOperationCombo(cbOperation);
		append(tbValueX);
		append(tbValueY);
		append(tbValueZ);
		buttonDelete.setParam(this);
		buttonDelete.setOnClick(Gesture_buttonDelete_onClick);
		append(buttonDelete);

		buttonDelete.setText(controller.m_dictionary.getString(Dictionary::DefaultActionDelete));

		setAlignment(Layout::center_align);
	}

	void ActionsController::GestureRow::removeSelf() {
		m_controller.removeGestureRow(this);
	}


	static void buttonBack_onClick(Button &button) {
		ActionsController *controller = (ActionsController*)button.param();
		controller->m_cineticoUI.goTo(CineticoUI::EXERCISES);
	}

	static void buttonAdd_onClick(Button& button) {
		ActionsController *controller = (ActionsController*)button.param();
		controller->setEditionMode(1);
	}

	static void buttonEdit_onClick(Button& button) {
		ActionsController *controller = (ActionsController*)button.param();
		if (controller->m_currentAction)
			controller->setEditionMode(2);
	}

	static void buttonDelete_onClick(Button& button) {
		ActionsController *controller = (ActionsController*)button.param();
		if (controller->m_currentAction) {
			Message::message_result res = Message::question(NULL, controller->m_dictionary.getString(Dictionary::DefaultActionQuestionDelete));
			if (res == Message::yes) {
				controller->m_cinetico.cineticoDB()->actionDAO()->exclude(*controller->m_currentAction);
				controller->updateActionList();
			}
		}
	}

	static void gridActions_onChange(GridView &grid, int row, int col) {
		ActionsController *controller = (ActionsController*)grid.param();
		controller->m_currentActionSelection = row;
		if (row >= 0)
			controller->m_currentAction = (Action*)grid.item(row, 0)->data();
	}


	static void buttonSaveAction_onClick(Button &button) {
		ActionsController *controller = (ActionsController*)button.param();
		controller->saveCurrentAction();
	}

	static void buttonCancelAction_onClick(Button &button) {
		ActionsController *controller = (ActionsController*)button.param();
		controller->setEditionMode(0);
	}

	static void comboActionType_onChange(uilib::ComboBox &combo, ComboBoxItem *item) {
		ActionsController *controller = (ActionsController*)combo.param();
		controller->m_currentActionTypeSelection = combo.selection();
	}

	void ActionsController::fillTransitionTypeCombo(cComboBox &combo) {
		combo.clear();
		combo.appendItem(m_dictionary.getString(Dictionary::SimpleGestureTransitionTypeFree),SimpleGesture::Free);
		combo.appendItem(m_dictionary.getString(Dictionary::SimpleGestureTransitionTypeDefinedMovement),SimpleGesture::FixedMovement);
	}

	void ActionsController::fillBodyPointCombo(cComboBox &combo) {
		std::vector<uilib::string> bpNames = m_cinetico.getAllBodyPointNames();
		combo.clear();
		for (unsigned int i = 0; i < bpNames.size(); ++i) {
			combo.appendItem(bpNames[i], i);
		}
	}

	void ActionsController::fillRefPointCombo(cComboBox &combo) {
		combo.clear();
		combo.appendItem(m_dictionary.getString(Dictionary::SimpleGestureRefPointAny), -2);
		combo.appendItem(m_dictionary.getString(Dictionary::SimpleGestureRefPointLastPosition), -1);
		std::vector<uilib::string> bpNames = m_cinetico.getAllBodyPointNames();
		for (unsigned int i = 0; i < bpNames.size(); ++i) {
			combo.appendItem(bpNames[i], i);
		}
	}

	static void buttonAddGesture_onClick(Button &button) {
		ActionsController *controller = (ActionsController*)button.param();
		controller->addGestureRow();
	}

	void buttonDeleteAllGestures_onClick(Button &button) {
		ActionsController *controller = (ActionsController*)button.param();
		controller->removeAllGestures();
	}

	/*
	static void buttonAddGesture_onClick(Button &button) {
		ActionsController *controller = (ActionsController*)button.param();
		SimpleGesture *gesture;
		SimpleGesture::TransitionType type = (SimpleGesture::TransitionType)(int)controller->cbTransitionType.item(controller->cbTransitionType.selection())->data();
		BodyPoint::BodyPart bp = (BodyPoint::BodyPart)(int)controller->cbBodyPoint.item(controller->cbBodyPoint.selection())->data();
		int refPoint = (int)controller->cbRefPoint.item(controller->cbRefPoint.selection())->data();
		unsigned long operation = (unsigned long)controller->cbOperation.item(controller->cbOperation.selection())->data();
		float valueX = controller->tbValueX.text().toFloat();
		float valueY = controller->tbValueY.text().toFloat();
		float valueZ = controller->tbValueZ.text().toFloat();

		if (type == SimpleGesture::FixedMovement) {
			//todo: get movementType, minSpeed, maxSpeed from fields
			MovementGesture::MovementType movementType = MovementGesture::Linear;
			float minSpeed = 0.f;
			float maxSpeed = 0.f;
			MovementGesture *movementGesture = new MovementGesture(bp, movementType);
			movementGesture->setMinSpeed(minSpeed);
			movementGesture->setMaxSpeed(maxSpeed);
			gesture = movementGesture;
		}
		else {
			gesture = new SimpleGesture(type, bp);
		}

		gesture->setRefPoint(refPoint);
		gesture->setOperation(operation);
		controller->m_gestures.push_back(gesture);
	}
	*/

	void ActionsController::fillOperationCombo(cComboBox &combo) {
		combo.clear();

		combo.appendItem(m_dictionary.getString(Dictionary::SimpleGestureOperationFixedPosition), SimpleGesture::makeOperationFlags(SimpleGesture::FixedPosition,true,true,true));
		combo.appendItem(m_dictionary.getString(Dictionary::SimpleGestureOperationInFront), SimpleGesture::makeOperationFlags(SimpleGesture::InFront,false,false,true));
		combo.appendItem(m_dictionary.getString(Dictionary::SimpleGestureOperationBehind), SimpleGesture::makeOperationFlags(SimpleGesture::Behind, false, false, true));
		combo.appendItem(m_dictionary.getString(Dictionary::SimpleGestureOperationToRight), SimpleGesture::makeOperationFlags(SimpleGesture::ToRight, true, false, false));
		combo.appendItem(m_dictionary.getString(Dictionary::SimpleGestureOperationToLeft), SimpleGesture::makeOperationFlags(SimpleGesture::ToLeft, true, false, false));
		combo.appendItem(m_dictionary.getString(Dictionary::SimpleGestureOperationAbove), SimpleGesture::makeOperationFlags(SimpleGesture::Above, false, true, false));
		combo.appendItem(m_dictionary.getString(Dictionary::SimpleGestureOperationBelow), SimpleGesture::makeOperationFlags(SimpleGesture::Below, false, true, false));
		combo.appendItem(m_dictionary.getString(Dictionary::SimpleGestureOperationAtSameBreadth), SimpleGesture::makeOperationFlags(SimpleGesture::AtSameBreadth, true, false, false));
		combo.appendItem(m_dictionary.getString(Dictionary::SimpleGestureOperationAtSameHeight), SimpleGesture::makeOperationFlags(SimpleGesture::AtSameHeight, false, true, false));
		combo.appendItem(m_dictionary.getString(Dictionary::SimpleGestureOperationAtSameDepth), SimpleGesture::makeOperationFlags(SimpleGesture::AtSameDepth, false, false, true));
		combo.appendItem(m_dictionary.getString(Dictionary::SimpleGestureOperationFixedOrientation), SimpleGesture::makeOperationFlags(SimpleGesture::FixedOrientation, true, true, true));
		combo.appendItem(m_dictionary.getString(Dictionary::SimpleGestureOperationOrientationLookingTo), SimpleGesture::makeOperationFlags(SimpleGesture::OrientationLookingTo, false, false, false));
	}

	void ActionsController::fillMovementTypeCombo(cComboBox &combo) {
		combo.clear();
		combo.appendItem(m_dictionary.getString(Dictionary::MovementGestureMovementTypeLinear),MovementGesture::Linear);
		combo.appendItem(m_dictionary.getString(Dictionary::MovementGestureMovementTypeSmooth),MovementGesture::Smooth);
	}

	bool ActionsController::validateFields() {
		bool required = false;
		bool invalid = false;

		if (tbName.text() == "") {
			required = true;
		}

		if (required) {
			Message::warning(NULL, m_dictionary.getString(Dictionary::ViewDefaultErrorRequiredFields));
		}
		else if (invalid) {
			//todo: campos somente numéricos
		}

		return !required && !invalid;
	}

	//todo: remove this temp shit
	void ActionsController::addGestureToAction(GestureRow *gestureRow) {
		Action *action = m_currentAction;

		if (!action)
			return;

		SimpleGesture *gesture;
		SimpleGesture::TransitionType type = (SimpleGesture::TransitionType)(int)gestureRow->cbTransitionType.item(gestureRow->cbTransitionType.selection())->data();
		BodyPoint::BodyPart bp = (BodyPoint::BodyPart)(int)gestureRow->cbBodyPoint.item(gestureRow->cbBodyPoint.selection())->data();
		int refPoint = (int)gestureRow->cbRefPoint.item(gestureRow->cbRefPoint.selection())->data();
		unsigned long operation = (unsigned long)gestureRow->cbOperation.item(gestureRow->cbOperation.selection())->data();
		float valueX = gestureRow->tbValueX.text().toFloat();
		float valueY = gestureRow->tbValueY.text().toFloat();
		float valueZ = gestureRow->tbValueZ.text().toFloat();

		if (type == SimpleGesture::FixedMovement) {
			//todo: get movementType, minSpeed, maxSpeed from fields
			MovementGesture::MovementType movementType = MovementGesture::Linear;
			float minSpeed = 0.f;
			float maxSpeed = 0.f;
			MovementGesture *movementGesture = new MovementGesture(bp, movementType);
			movementGesture->setMinSpeed(minSpeed);
			movementGesture->setMaxSpeed(maxSpeed);
			gesture = movementGesture;
		}
		else {
			gesture = new SimpleGesture(type, bp);
		}

		gesture->setRefPoint(refPoint);
		gesture->setOperation(operation);

		action->addGesture(gesture);
	}
	void ActionsController::saveCurrentAction() {

		if(!validateFields())
			return;

		if (layoutGestures.itemCount() == 0) {
			uilib::Message::warning(NULL, "Uma ação deve conter pelo menos um gesto.");
			return;
		}

		string &name = tbName.text();
		float minTimeStr = tbMinTime.text().toFloat();
		float maxTimeStr = tbMaxTime.text().toFloat();
		float timeToHold = tbTimeToHold.text().toFloat();

		Action *action = m_currentAction;

		action->setName(name.data());
		action->setMinExecutionTime(minTimeStr);
		action->setMaxExecutionTime(maxTimeStr);
		action->setTimeToHold(timeToHold);

		for (int i = 0; i < layoutGestures.itemCount(); ++i) {
			LayoutItem *layoutItem = layoutGestures.item(i);
			GestureRow *gestureRow = (GestureRow*)layoutItem->layout();
			addGestureToAction(gestureRow);
		}

		for (unsigned int i = 0; i < m_gestures.size(); ++i) {
			action->addGesture(m_gestures[i]);
		}

		if (m_editMode == 1) {
			m_cinetico.cineticoDB()->actionDAO()->create(*action);
			delete action;
		}
		else {
			m_cinetico.cineticoDB()->actionDAO()->update(*action);
		}

		if(!m_gestures.empty())
			m_gestures.clear();

		setEditionMode(0);
	}

	void ActionsController::onViewUpdate() {
		title.setTitle(m_dictionary.getString(Dictionary::ActionsViewTitle));
		title.setDesc(m_dictionary.getString(Dictionary::ActionsViewDesc));

		buttonAdd.setText(m_dictionary.getString(Dictionary::DefaultActionAdd));
		buttonBack.setText(m_dictionary.getString(Dictionary::DefaultActionBack));

		gridActions.setHeaderText(0, m_dictionary.getString(Dictionary::ActionName));
		gridActions.setHeaderText(2, m_dictionary.getString(Dictionary::ActionMinExecutionTime));
		gridActions.setHeaderText(3, m_dictionary.getString(Dictionary::ActionMaxExecutionTime));
		gridActions.setHeaderText(4, m_dictionary.getString(Dictionary::ActionTimeToHold));
		gridActions.setHeaderText(5, m_dictionary.getString(Dictionary::ActionGestureCount));

		buttonEdit.setText(m_dictionary.getString(Dictionary::DefaultActionEdit));
		buttonDelete.setText(m_dictionary.getString(Dictionary::DefaultActionDelete));

		buttonSaveAction.setText(m_dictionary.getString(Dictionary::DefaultActionSave));
		buttonCancelAction.setText(m_dictionary.getString(Dictionary::DefaultActionCancel));

		separatorActionBasicData.setText(m_dictionary.getString(Dictionary::ActionsViewSectionBasicData));

		tbName.setLabel(m_dictionary.getString(Dictionary::ActionName) + '*');
		tbMinTime.setLabel(m_dictionary.getString(Dictionary::ActionMinExecutionTime));
		tbMaxTime.setLabel(m_dictionary.getString(Dictionary::ActionMaxExecutionTime));
		tbTimeToHold.setLabel(m_dictionary.getString(Dictionary::ActionTimeToHold));

		separatorGestureData.setText("Gestos");

		labelTransitionType.setText(m_dictionary.getString(Dictionary::SimpleGestureTransitionType) + '*');
		labelBodyPoint.setText(m_dictionary.getString(Dictionary::SimpleGestureBodyPoint) + '*');
		labelRefPoint.setText(m_dictionary.getString(Dictionary::SimpleGestureRefPoint) + '*');
		labelOperation.setText(m_dictionary.getString(Dictionary::SimpleGestureOperation) + '*');
		labelValueX.setText(m_dictionary.getString(Dictionary::SimpleGestureValueX));
		labelValueY.setText(m_dictionary.getString(Dictionary::SimpleGestureValueY));
		labelValueZ.setText(m_dictionary.getString(Dictionary::SimpleGestureValueZ));
		buttonDummy.setText(m_dictionary.getString(Dictionary::DefaultActionDelete));
		buttonDummy.setParam(this);
		buttonDummy.setOnClick(buttonDeleteAllGestures_onClick);

		buttonAddGesture.setText(m_dictionary.getString(Dictionary::DefaultActionAdd));
	}

	ActionsController::ActionsController(CineticoUI &cineticoUI)
		: Controller(cineticoUI)
	{
		
		buttonAdd.setParam(this);
		buttonAdd.setOnClick(buttonAdd_onClick);
		
		buttonBack.setParam(this);
		buttonBack.setOnClick(buttonBack_onClick);
		layoutActionButtons.append(buttonAdd);
		layoutActionButtons.append(buttonBack);

		//List
		gridActions.setStyle(CS_Border);
		gridActions.setColumnCount(5);
		gridActions.setHeaderVisible(true);

		gridActions.setParam(this);
		gridActions.setOnItemSelect(gridActions_onChange);
		
		buttonEdit.setParam(this);
		buttonEdit.setOnClick(buttonEdit_onClick);
		
		buttonDelete.setParam(this);
		buttonDelete.setOnClick(buttonDelete_onClick);

		layoutGridActions.append(buttonEdit);
		layoutGridActions.append(buttonDelete);
		layoutContentList.append(gridActions, MaximumSize);
		layoutContentList.append(layoutGridActions);
		layoutContentList.setAlignment(Layout::center_align);
		//

		//Action data

		//Action buttons
		
		buttonSaveAction.setParam(this);
		buttonSaveAction.setOnClick(buttonSaveAction_onClick);
		
		buttonCancelAction.setParam(this);
		buttonCancelAction.setOnClick(buttonCancelAction_onClick);
		layoutActionDataActionButtons.append(buttonSaveAction);
		layoutActionDataActionButtons.append(buttonCancelAction);

		//Base Action data
		buttonAddGesture.setParam(this);
		buttonAddGesture.setOnClick(buttonAddGesture_onClick);

		layoutActionDataRow1.append(tbName);
		layoutActionDataRow1.append(tbMinTime);
		layoutActionDataRow1.append(tbMaxTime);
		layoutActionDataRow1.append(tbTimeToHold);

		layoutGestureLabels.append(labelTransitionType,Size(SizeTypeMax,SizeTypeAuto));
		layoutGestureLabels.append(labelBodyPoint, Size(SizeTypeMax, SizeTypeAuto));
		layoutGestureLabels.append(labelRefPoint, Size(SizeTypeMax, SizeTypeAuto));
		layoutGestureLabels.append(labelOperation, Size(SizeTypeMax, SizeTypeAuto));
		layoutGestureLabels.append(labelValueX, Size(SizeTypeMax, SizeTypeAuto));
		layoutGestureLabels.append(labelValueY, Size(SizeTypeMax, SizeTypeAuto));
		layoutGestureLabels.append(labelValueZ, Size(SizeTypeMax, SizeTypeAuto));
		buttonDummy.setVisible(false);
		layoutGestureLabels.append(buttonDummy);

		layoutActionData.append(layoutActionDataActionButtons);
		layoutActionData.append(separatorActionBasicData);
		layoutActionData.append(layoutActionDataRow1);
		layoutActionData.append(separatorGestureData);
		layoutActionData.append(buttonAddGesture);
		layoutActionData.append(layoutGestureLabels);
		layoutActionData.append(layoutGestures);
		

		//
		layoutContent.append(layoutContentList);

		layout.append(title);
		layout.append(layoutActionButtons);
		layout.append(separatorContent);
		layout.append(layoutContent);
		layout.setMargin(10);
	}

	Layout *ActionsController::viewDefinition() {
		return &layout; 
	}

	void ActionsController::onViewEnter(ViewParams params) {
		m_editMode = 0;
		m_currentActionTypeSelection = -1;
		m_currentMovementTypeSelection = -1;
		buttonEdit.setEnabled(false);

		Exercise *exercise = params.get<Exercise*>("exercise");
		m_currentExercise = exercise;
		updateActionList();
	}

	void ActionsController::onViewTick() {
		static int lastEditMode = 0;
		static int lastActionSelection = -1;
		static int lastActionType = -1;

		if (m_editMode != lastEditMode) {
			buttonAdd.setEnabled(m_editMode == 0);
			buttonBack.setEnabled(m_editMode == 0);
		}

		if (m_currentActionSelection != lastActionSelection) {
			buttonEdit.setEnabled(m_currentActionSelection >= 0);
		}

		if (m_currentActionTypeSelection != lastActionType) {
			//todo: dinamismo
			/*
			if (lastActionType == Action::Position) {
				layoutSpecific.remove(layoutPositionSpecific);
			}
			else if (lastActionType == Action::Movement) {
				layoutSpecific.remove(layoutMovementSpecific);
			}

			if (m_currentActionTypeSelection == Action::Position) {
				layoutSpecific.append(layoutPositionSpecific);
			}
			else if (m_currentActionTypeSelection == Action::Movement) {
				layoutSpecific.append(layoutMovementSpecific);
			}
			*/

			layout.setSize(layout.size());
			layout.setVisible(true);
		}

		lastEditMode = m_editMode;
		lastActionSelection = m_currentActionSelection;
		lastActionType = m_currentActionTypeSelection;
	}

	void ActionsController::onViewQuit() {
		removeAllGestures();
	}

	void ActionsController::addGestureRow() {
		GestureRow *gestureRow = new GestureRow(*this);
		layoutGestures.append(*gestureRow);
		layout.setSize(layout.size());
	}

	void ActionsController::removeGestureRow(ActionsController::GestureRow *gestureRow) {
		layoutGestures.remove(*gestureRow);
		delete gestureRow;
		layout.setSize(layout.size());
	}

	void ActionsController::removeAllGestures() {
		for (int i = layoutGestures.itemCount() -1; i >= 0; --i) {
			GestureRow *gestureRow = (GestureRow*)layoutGestures.item(i)->layout();
			layoutGestures.remove(*gestureRow);
			delete gestureRow;
		}
		//update layout
		layout.setSize(layout.size());
	}

	void ActionsController::setEditionMode(int mode) {
		if (m_editMode == 0)
			layoutContent.remove(layoutContentList);
		else if (m_editMode == 1 || m_editMode == 2) {
			layoutContent.remove(layoutActionData);
		}

		if (mode == 0) {
			layoutContent.append(layoutContentList);
			updateActionList();
		}
		else if (mode == 1 || mode == 2) {
			layoutContent.append(layoutActionData);
			//
			tbName.setText("");
			tbMinTime.setText("");
			tbMaxTime.setText("");
			tbTimeToHold.setText("");

			if (mode == 2) {
				Action *action = m_currentAction;
				tbName.setText(action->name().c_str());
				tbMinTime.setText(string::fromFloat(action->minExecutionTime()).data());
				tbMaxTime.setText(string::fromFloat(action->maxExecutionTime()).data());
				tbTimeToHold.setText(string::fromFloat(action->timeToHold()).data());
				for (int i = 0; i < action->gestureCount(); ++i) {
					GestureRow *gestureRow = new GestureRow(*this);
					SimpleGesture *gesture = action->gesture(i);
					//gestureRow->cbTransitionType.setSelection(); //todo
					//gestureRow->cbBodyPoint.setSelection();
					//gestureRow->cbBodyPoint.setSelection();
					//gestureRow->cbBodyPoint.setSelection();
					gestureRow->tbValueX.setText(string::fromFloat(gesture->value().x()));
					gestureRow->tbValueY.setText(string::fromFloat(gesture->value().y()));
					gestureRow->tbValueZ.setText(string::fromFloat(gesture->value().z()));
					layoutGestures.append(*gestureRow);
				}
				layout.setSize(layout.size());

			}
			else {
				m_currentAction = new Action(*m_currentExercise);
			}
		}

		m_editMode = mode;
		layout.setSize(layout.size());
		layout.setVisible(true);
	}

	void ActionsController::updateActionList() {
		std::vector<Action*> actionList = m_cinetico.cineticoDB()->actionDAO()->getAllActionsByExercise(*m_currentExercise);
		gridActions.deleteRows();
		for (unsigned int i = 0; i < actionList.size(); ++i) {
			Action *action = actionList[i];
			gridActions.insertRow();
			int lastRow = gridActions.rowCount() - 1;
			gridActions.setItem(lastRow, 0, new ListViewItem(action->name().c_str(),uilib::Color(0,0,0),uilib::FontDesc("Arial",10,0),action));
			gridActions.setItem(lastRow, 1, new ListViewItem(string::fromFloat(action->minExecutionTime(), 2)));
			gridActions.setItem(lastRow, 2, new ListViewItem(string::fromFloat(action->maxExecutionTime(), 2)));
			gridActions.setItem(lastRow, 3, new ListViewItem(string::fromFloat(action->timeToHold(), 2)));
			gridActions.setItem(lastRow, 4, new ListViewItem(string::fromInteger(action->gestureCount())));
		}
		m_currentActionSelection = -1;
		m_currentAction = NULL;
	}

}