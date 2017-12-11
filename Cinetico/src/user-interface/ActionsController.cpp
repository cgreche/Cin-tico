
#include "cinetico.h"
#include "cineticodb.h"
#include "cineticoui.h"

#include "core/entity/Action.h"

#include "ActionsController.h"

#include <sstream>

namespace cinetico {
	using namespace cinetico_core;
	using cinetico::ComboBox;

	static int MaxGestureItemWidth = UITools::getTextSize("999", FontDesc("Arial", 10, 0)).width()+6;

	ActionsController::GestureItem::GestureItem(ActionsController &controller)
		: m_controller(controller) {
		m_color = uilib::Color(255, 255, 255);
		m_colorSimpleGesture = uilib::Color(0x80, 0xDE, 0xEA);
		m_colorMovementGesture = uilib::Color(0xFF, 0xCC, 0x80);
		m_state = IDLE;
		setBackgroundColor(m_color);
	}

	Size ActionsController::GestureItem::getAutoSize() {
		return Size(MaxGestureItemWidth, MaxGestureItemWidth * 3);
	}

	void ActionsController::GestureItem::onMouseEnterEvent(MouseEvent &event) {
		m_state = HOVER;
		update();
	}

	void ActionsController::GestureItem::onMouseLeaveEvent(MouseEvent &event) {
		m_state = IDLE;
		update();
	}

	void ActionsController::GestureItem::onMousePressEvent(MouseEvent &event) {
		m_state = PRESSED;
		update();
	}

	void ActionsController::GestureItem::onMouseReleaseEvent(MouseEvent &event) {
		m_state = HOVER;
		setSelected(true);
	}

	void ActionsController::GestureItem::setSelected(bool selected) {
		if (m_selected != selected) {
			m_selected = selected;
			if (selected)
				m_controller.onGestureItemSelected(*this);
		}
		update();
	}

	void ActionsController::GestureItem::setParam(void *param) {
		m_param = param;
		update();
	}

	void ActionsController::GestureItem::update() {
		uilib::Color color = m_color;
		if (m_param != NULL) {
			SimpleGesture *gesture = (SimpleGesture*)m_param;
			if (gesture->transitionType() == SimpleGesture::Free)
				color = m_colorSimpleGesture;
			else if (gesture->transitionType() == SimpleGesture::FixedMovement)
				color = m_colorMovementGesture;
		}

		if (m_state == IDLE) {
			if (m_selected) {
				color = uilib::Color(color.red()*0.8f, color.green()*0.8f, color.blue()*0.8f);
			}
		} 
		else if (m_state == HOVER) {
			color = uilib::Color(color.red()*0.9f, color.green()*0.9f, color.blue()*0.9f);
		}
		else if (m_state == PRESSED) {
			color = uilib::Color(color.red()*0.7f, color.green()*0.7f, color.blue()*0.7f);
		}

		setBackgroundColor(color);
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
		if (controller->m_currentActionSelection >= 0)
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
		ActionsController::GestureItem *item = controller->addNewGesture(NULL);
		item->setSelected(true);
	}

	static void buttonDelGesture_onClick(Button &button) {
		ActionsController *controller = (ActionsController*)button.param();

		int selectedGesture = controller->m_selectedGesture;
		if (selectedGesture == -1)
			return;

		ActionsController::GestureItem *item = controller->gestureItems[selectedGesture];
		controller->layoutGestureItems.remove(*item);
		controller->gestureItems.erase(controller->gestureItems.begin() + selectedGesture);
		delete item;

		if (selectedGesture < (int)controller->gestureItems.size()) {
			for (int i = selectedGesture; i < (int)controller->gestureItems.size(); ++i) {
				controller->gestureItems[i]->setText(string::fromInteger(i+1));
			}
			controller->gestureItems[selectedGesture]->setSelected(true);
		}
		else {
			selectedGesture = -1;
		}

		controller->layoutGestureItems.update();	
	}

	void buttonDeleteAllGestures_onClick(Button &button) {
		ActionsController *controller = (ActionsController*)button.param();
		controller->removeAllGestures();
	}

	static void buttonSaveGesture_onClick(Button &button) {
		ActionsController *controller = (ActionsController*)button.param();
		controller->saveCurrentGesture();
	}

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

		//todo: add to dictionary
		if (layoutGesture.itemCount() == 0) {
			uilib::Message::warning(NULL, "Uma ação deve conter pelo menos um gesto.");
			return false;
		}

		//validate gestures
		std::vector<int> invalidGestures;
		for (int i = 0; i < (int)gestureItems.size(); ++i) {
			if (gestureItems[i]->param() == NULL) {
				invalidGestures.push_back(i+1);
			}
		}

		if (invalidGestures.size() > 0) {
			string str = "A(s) Ação(ões) ";
			size_t i;
			for (i = 0; i < invalidGestures.size() - 1; ++i) {
				str += string::fromInteger(invalidGestures[i]);
				str += ", ";
			}
			str += string::fromInteger(invalidGestures[i]);
			str += " estão inválidas";
			Message::warning(NULL,str);
			return false;
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

		Action *action = new Action(*m_currentExercise, m_currentAction ? m_currentAction->id() : -1);
		action->setName(name.data());
		action->setMinExecutionTime(minTimeStr);
		action->setMaxExecutionTime(maxTimeStr);
		action->setTimeToHold(timeToHold);

		for (GestureItem *item : gestureItems) {
			action->addGesture((SimpleGesture*)item->param());
		}

		if (m_editMode == 1) {
			m_cinetico.cineticoDB()->actionDAO()->create(*action);
		}
		else {
			m_cinetico.cineticoDB()->actionDAO()->update(*action);
		}
		delete action;

		setEditionMode(0);
		
	}

	bool ActionsController::validateCurrentGesture() {
		if (m_selectedGesture == -1)
			return false;
		if (cbTransitionType.selectedItem() == NULL
			|| cbBodyPoint.selectedItem() == NULL
			|| cbRefPoint.selectedItem() == NULL
			|| cbOperation.selectedItem() == NULL) {
			return false;
		}
		return true;
	}

	void ActionsController::saveCurrentGesture() {
		if (m_selectedGesture == -1)
			return;

		GestureItem *item = gestureItems[m_selectedGesture];

		if (!validateCurrentGesture())
			return;

		SimpleGesture *gesture;
		SimpleGesture::TransitionType type = (SimpleGesture::TransitionType)(int)cbTransitionType.selectedItem()->data();
		BodyPoint::BodyPart bp = (BodyPoint::BodyPart)(int)cbBodyPoint.selectedItem()->data();
		int refPoint = (int)cbRefPoint.selectedItem()->data();
		unsigned long operation = (unsigned long)cbOperation.selectedItem()->data();
		float valueX = tbValueX.text().toFloat();
		float valueY = tbValueY.text().toFloat();
		float valueZ = tbValueZ.text().toFloat();

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

		if (item->param()) {
			delete (SimpleGesture*)item->param();
		}

		item->setParam(gesture);
		item->update();
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

		//todo: add to dictionary
		separatorGestureData.setText("Gestos");

		cbTransitionType.setLabel(m_dictionary.getString(Dictionary::SimpleGestureTransitionType) + '*');
		cbBodyPoint.setLabel(m_dictionary.getString(Dictionary::SimpleGestureBodyPoint) + '*');
		cbRefPoint.setLabel(m_dictionary.getString(Dictionary::SimpleGestureRefPoint) + '*');
		cbOperation.setLabel(m_dictionary.getString(Dictionary::SimpleGestureOperation) + '*');
		tbValueX.setLabel(m_dictionary.getString(Dictionary::SimpleGestureValueX));
		tbValueY.setLabel(m_dictionary.getString(Dictionary::SimpleGestureValueY));
		tbValueZ.setLabel(m_dictionary.getString(Dictionary::SimpleGestureValueZ));
		buttonSaveGesture.setText(m_dictionary.getString(Dictionary::DefaultActionSave));

		buttonAddGesture.setText(m_dictionary.getString(Dictionary::DefaultActionAdd));
		buttonDelGesture.setText(m_dictionary.getString(Dictionary::DefaultActionDelete));
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

		buttonAddGesture.setParam(this);
		buttonAddGesture.setOnClick(buttonAddGesture_onClick);
		buttonDelGesture.setParam(this);
		buttonDelGesture.setOnClick(buttonDelGesture_onClick);

		//Base Action data
		layoutActionDataRow1.append(tbName);
		layoutActionDataRow1.append(tbMinTime);
		layoutActionDataRow1.append(tbMaxTime);
		layoutActionDataRow1.append(tbTimeToHold);
		layoutActionCommands.append(buttonAddGesture);
		layoutActionCommands.append(buttonDelGesture);



		//Gestures
		fillTransitionTypeCombo(cbTransitionType);
		fillBodyPointCombo(cbBodyPoint);
		fillRefPointCombo(cbRefPoint);
		fillOperationCombo(cbOperation);
		buttonSaveGesture.setOnClick(buttonSaveGesture_onClick);
		buttonSaveGesture.setParam(this);

		layoutGesture.append(cbTransitionType,Size(SizeTypeMax,SizeTypeAuto));
		layoutGesture.append(cbBodyPoint, Size(SizeTypeMax, SizeTypeAuto));
		layoutGesture.append(cbRefPoint, Size(SizeTypeMax, SizeTypeAuto));
		layoutGesture.append(cbOperation, Size(SizeTypeMax, SizeTypeAuto));
		layoutGesture.append(tbValueX, Size(SizeTypeMax, SizeTypeAuto));
		layoutGesture.append(tbValueY, Size(SizeTypeMax, SizeTypeAuto));
		layoutGesture.append(tbValueZ, Size(SizeTypeMax, SizeTypeAuto));
		layoutGesture.append(buttonSaveGesture);

		layoutActionData.append(layoutActionDataActionButtons);
		layoutActionData.append(separatorActionBasicData);
		layoutActionData.append(layoutActionDataRow1);
		layoutActionData.append(separatorGestureData);
		layoutActionData.append(layoutActionCommands);
		layoutActionData.append(layoutGestureItems,Size(SizeTypeMax, MaxGestureItemWidth*3));
		layoutActionData.append(layoutGesture);
		

		//
		layoutContent.append(layoutContentList);

		layout.append(title);
		layout.append(layoutActionButtons);
		layout.append(separatorContent);
		layout.append(layoutContent);
		layout.setMargin(10);

		m_selectedGesture = -1;
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

	void ActionsController::onGestureItemSelected(GestureItem &item) {

		//find index;
		int selectedGesture = -1;
		for (int i = 0; i < (int)gestureItems.size(); ++i) {
			GestureItem *_item = gestureItems[i];
			if (&item != _item)
				_item->setSelected(false);
			else
				selectedGesture = i;
		}
		m_selectedGesture = selectedGesture;

		if (selectedGesture == -1) {
			//todo: disable fields?
		}

		SimpleGesture *gesture = (SimpleGesture*)item.param();

		//clearing fields
		cbTransitionType.setSelection(-1);
		cbBodyPoint.setSelection(-1);
		cbRefPoint.setSelection(-1);
		cbOperation.setSelection(-1);
		tbValueX.setText("");
		tbValueY.setText("");
		tbValueZ.setText("");

		if (!gesture)
			return;

		//todo: set fields
		cbTransitionType.setSelection(gesture->transitionType());
		cbBodyPoint.setSelection(gesture->bodyPoint()); 
		cbRefPoint.setSelectionByData((void*)gesture->refPoint());
		cbOperation.setSelectionByData((void*)gesture->operation());
		tbValueX.setText(string::fromFloat(gesture->value().x()));
		tbValueY.setText(string::fromFloat(gesture->value().y()));
		tbValueZ.setText(string::fromFloat(gesture->value().z()));

		if (gesture->transitionType() == SimpleGesture::FixedMovement) {
			MovementGesture *movementGesture = reinterpret_cast<MovementGesture*>(gesture);
			//todo: set movement gesture fields
		}

	}

	ActionsController::GestureItem *ActionsController::addNewGesture(SimpleGesture* gesture) {
		ActionsController::GestureItem *item = new ActionsController::GestureItem(*this);
		item->setText(string::fromInteger(layoutGestureItems.itemCount() + 1));
		item->setAlignment(Label::HCenter | Label::VCenter);
		item->setStyle(item->style() | CS_Border);
		item->setTransparent(false);
		if(gesture)
			item->setParam(gesture);
		gestureItems.push_back(item);
		layoutGestureItems.append(*item);
		layoutGestureItems.update();
		return item;
	}

	void ActionsController::removeAllGestures() {
		/*
		for (int i = layoutGestures.itemCount() -1; i >= 0; --i) {
			GestureRow *gestureRow = (GestureRow*)layoutGestures.item(i)->layout();
			layoutGestures.remove(*gestureRow);
			delete gestureRow;
		}
		*/
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
			
			// Clear gestures from last edit operation
			if (!gestureItems.empty()) {
				for (int i = gestureItems.size() - 1; i >= 0; --i) {
					GestureItem *item = gestureItems[i];
					layoutGestureItems.remove(*item);
				}
				gestureItems.clear();
			}
			m_selectedGesture = -1;

			if (mode == 2) {
				//Editing an existing action, make a copy
				Action *action = (Action*)m_currentAction;
				tbName.setText(action->name().c_str());
				tbMinTime.setText(string::fromFloat(action->minExecutionTime()).data());
				tbMaxTime.setText(string::fromFloat(action->maxExecutionTime()).data());
				tbTimeToHold.setText(string::fromFloat(action->timeToHold()).data());

				for (int i = 0; i < action->gestureCount(); ++i) {
					SimpleGesture *gesture = action->gesture(i);
					SimpleGesture *copy;
					if (gesture->transitionType() == SimpleGesture::FixedMovement) {
						MovementGesture *_copy = new MovementGesture(gesture->bodyPoint(), ((MovementGesture*)gesture)->movementType());
						_copy->setMinSpeed(((MovementGesture*)gesture)->minSpeed());
						_copy->setMaxSpeed(((MovementGesture*)gesture)->maxSpeed());
						//todo: handle extraPoint;
						copy = _copy;
					}
					else {
						copy = new SimpleGesture(SimpleGesture::Free, gesture->bodyPoint());
					}
					copy->setRefPoint(gesture->refPoint());
					copy->setOperation(gesture->operation());
					copy->setValue(gesture->value());

					GestureItem *item = addNewGesture(copy);
				}

				layout.setSize(layout.size());
			}
			else {
				//inserting new
				tbName.setText("");
				tbMinTime.setText("");
				tbMaxTime.setText("");
				tbTimeToHold.setText("");

				cbBodyPoint.setSelection(-1);
				cbTransitionType.setSelection(-1);
				cbRefPoint.setSelection(-1);
				cbOperation.setSelection(-1);
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