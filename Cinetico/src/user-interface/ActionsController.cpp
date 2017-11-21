
#include "cinetico.h"
#include "cineticodb.h"
#include "cineticoui.h"

#include "entity/core/Action.h"

#include "ActionsController.h"

#include <sstream>

namespace cinetico {
	using namespace cinetico_core;
	using cinetico::ComboBox;

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
		combo.appendItem("Livre",SimpleGesture::Free);
		combo.appendItem("Movimento fixo",SimpleGesture::FixedMovement);
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
		combo.appendItem(m_dictionary.getString(Dictionary::ActionRefPointAny), -2);
		combo.appendItem(m_dictionary.getString(Dictionary::ActionRefPointLastPosition), -1);
		std::vector<uilib::string> bpNames = m_cinetico.getAllBodyPointNames();
		for (unsigned int i = 0; i < bpNames.size(); ++i) {
			combo.appendItem(bpNames[i], i);
		}
	}

	static void buttonAddGesture_onClick(Button &button) {
		ActionsController *controller = (ActionsController*)button.param();
		SimpleGesture *gesture;
		SimpleGesture::TransitionType type = (SimpleGesture::TransitionType)(int)controller->cbTransitionType.item(controller->cbTransitionType.selection())->data();
		BodyPoint::BodyPart bp = (BodyPoint::BodyPart)(int)controller->cbBodyPoint.item(controller->cbBodyPoint.selection())->data();
		int refPoint = (int)controller->cbRefPoint.item(controller->cbRefPoint.selection())->data();
		unsigned long operation = (unsigned long)controller->cbOperation.item(controller->cbOperation.selection())->data();
		float valueX = controller->tbPositionX.text().toFloat();
		float valueY = controller->tbPositionY.text().toFloat();
		float valueZ = controller->tbPositionZ.text().toFloat();

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

	void ActionsController::fillOperationCombo(cComboBox &combo) {
		combo.clear();
		combo.appendItem("Posição fixa", SimpleGesture::makeOperationFlags(SimpleGesture::FixedPosition,true,true,true));
		combo.appendItem("À frente", SimpleGesture::makeOperationFlags(SimpleGesture::InFront,false,false,true));
		combo.appendItem("Atrás", SimpleGesture::makeOperationFlags(SimpleGesture::Behind, false, false, true));
		combo.appendItem("À direita", SimpleGesture::makeOperationFlags(SimpleGesture::ToRight, true, false, false));
		combo.appendItem("À esquerda", SimpleGesture::makeOperationFlags(SimpleGesture::ToLeft, true, false, false));
		combo.appendItem("Acima", SimpleGesture::makeOperationFlags(SimpleGesture::Above, false, true, false));
		combo.appendItem("Abaixo", SimpleGesture::makeOperationFlags(SimpleGesture::Below, false, true, false));
		combo.appendItem("Alinhado horizontalmente", SimpleGesture::makeOperationFlags((SimpleGesture::Operation)7, false, true, true));
		combo.appendItem("Alinhado verticalmente", SimpleGesture::makeOperationFlags((SimpleGesture::Operation)8, true, false, false));
		combo.appendItem("Na mesma linha frontal", SimpleGesture::makeOperationFlags((SimpleGesture::Operation)9, false, false, true));
	}

	void ActionsController::fillMovementTypeCombo(cComboBox &combo) {
		combo.appendItem("Linear",0);
		combo.appendItem("Angular",1);
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

	void ActionsController::saveCurrentAction() {

		if(!validateFields())
			return;

		string &name = tbName.text();
		float minTimeStr = tbMinTime.text().toFloat();
		float maxTimeStr = tbMaxTime.text().toFloat();
		float timeToHold = tbTimeToHold.text().toFloat();

		Action *action = m_currentAction;

		action->setName(name.data());
		action->setMinExecutionTime(minTimeStr);
		action->setMaxExecutionTime(maxTimeStr);
		action->setTimeToHold(timeToHold);

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
		gridActions.setHeaderText(5, "Gesture count");

		buttonEdit.setText(m_dictionary.getString(Dictionary::DefaultActionEdit));
		buttonDelete.setText(m_dictionary.getString(Dictionary::DefaultActionDelete));

		buttonSaveAction.setText(m_dictionary.getString(Dictionary::DefaultActionSave));
		buttonCancelAction.setText(m_dictionary.getString(Dictionary::DefaultActionCancel));

		separatorActionBasicData.setText(m_dictionary.getString(Dictionary::ActionsViewSectionBasicData));

		tbName.setLabel(m_dictionary.getString(Dictionary::ActionName) + "*");
		tbMinTime.setLabel(m_dictionary.getString(Dictionary::ActionMinExecutionTime));
		tbMaxTime.setLabel(m_dictionary.getString(Dictionary::ActionMaxExecutionTime));
		tbTimeToHold.setLabel(m_dictionary.getString(Dictionary::ActionTimeToHold));

		fillTransitionTypeCombo(cbTransitionType);
		fillBodyPointCombo(cbBodyPoint);
		fillRefPointCombo(cbRefPoint);
		fillOperationCombo(cbOperation);
		buttonAddGesture.setText(m_dictionary.getString(Dictionary::DefaultActionAdd));
		buttonCancelGesture.setText(m_dictionary.getString(Dictionary::DefaultActionCancel));
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
		buttonCancelGesture.setParam(this);


		layoutActionDataRow1.append(tbName);
		layoutActionDataRow1.append(tbMinTime);
		layoutActionDataRow1.append(tbMaxTime);
		layoutActionDataRow1.append(tbTimeToHold);

		layoutGestureDataRow1.append(cbTransitionType);
		layoutGestureDataRow1.append(cbBodyPoint);

		layoutGestureDataRow2.append(cbRefPoint);
		layoutGestureDataRow2.append(cbOperation);

		layoutGestureDataRow2.append(tbPositionX);
		layoutGestureDataRow2.append(tbPositionY);
		layoutGestureDataRow2.append(tbPositionZ);
		layoutGestureDataRow2.append(buttonAddGesture);
		layoutGestureDataRow2.append(buttonCancelGesture);

		layoutActionData.append(layoutActionDataActionButtons);
		layoutActionData.append(separatorActionBasicData);
		layoutActionData.append(layoutActionDataRow1);
		layoutActionData.append(separatorGestureData);
		layoutActionData.append(layoutGestureDataRow1);
		layoutActionData.append(layoutGestureDataRow2);
		layoutActionData.append(layoutGestureDataRow3);

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
				/*
				Action *action = m_currentAction;
				cbActionType.setSelection(action->type());
				cbOrderType.setSelection(action->order());
				tbName.setText(action->name().c_str());
				cbBodyPoint.setSelection(action->bodyPoint());
				cbRefPoint.setSelection(action->refPoint() + 2);
				int opFlags = action->operation();
				cbOperation.setSelection(opFlags&255);
				tbMinTime.setText(string::fromFloat(action->minTime()).data());
				tbMaxTime.setText(string::fromFloat(action->maxTime()).data());
				tbPositionX.setText(string::fromFloat(action->finalPosition().x()*100).data());
				tbPositionY.setText(string::fromFloat(action->finalPosition().y()*100).data());
				tbPositionZ.setText(string::fromFloat(action->finalPosition().z()*100).data());

				if (action->type() == Action::Position) {
					tbMinHoldTime.setText(string::fromFloat(((PositionAction*)action)->minHoldTime()).data());
				}
				else if (action->type() == Action::Movement) {
					cbMovementType.setSelection(((MovementAction*)action)->movementType());
					tbMinSpeed.setText(string::fromFloat(((MovementAction*)action)->minSpeed()).data());
					tbMinSpeed.setText(string::fromFloat(((MovementAction*)action)->maxSpeed()).data());
				}
				*/

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

			gridActions.setItem(lastRow, 0, new ListViewItem(action->name().c_str()));
			gridActions.setItem(lastRow, 1, new ListViewItem(string::fromFloat(action->minExecutionTime(), 2)));
			gridActions.setItem(lastRow, 2, new ListViewItem(string::fromFloat(action->maxExecutionTime(), 2)));
			gridActions.setItem(lastRow, 3, new ListViewItem(string::fromFloat(action->timeToHold(), 2)));
			gridActions.setItem(lastRow, 4, new ListViewItem(string::fromInteger(action->gestureCount())));
		}
		m_currentActionSelection = -1;
		m_currentAction = NULL;
	}

}