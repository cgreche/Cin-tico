
#include "cinetico.h"
#include "ExerciseManagementController.h"

#include "entity/core/PositionAction.h"
#include "entity/core/MovementAction.h"

#include <sstream>

namespace cinetico {
	using namespace cinetico_core;
	using cinetico::ComboBox;

	extern Cinetico g_cinetico;

	static void buttonBack_onClick(Button &button) {
		ExerciseManagementController *controller = (ExerciseManagementController*)button.param();
		g_cinetico.goTo(Cinetico::EXERCISES);
	}

	static void buttonAdd_onClick(Button& button) {
		ExerciseManagementController *controller = (ExerciseManagementController*)button.param();
		controller->setEditionMode(1);
	}

	static void buttonEdit_onClick(Button& button) {
		ExerciseManagementController *controller = (ExerciseManagementController*)button.param();
		if (controller->m_currentAction)
			controller->setEditionMode(2);
	}

	static void buttonDelete_onClick(Button& button) {
		ExerciseManagementController *controller = (ExerciseManagementController*)button.param();
		if (controller->m_currentAction) {
			Message::message_result res = Message::question(NULL, "Deseja excluir este registro?");
			if (res == Message::yes) {
				g_cinetico.cineticoDB()->actionDAO()->exclude(*controller->m_currentAction);
				controller->updateActionList();
			}
		}
	}

	static void gridActions_onChange(GridView &grid, int row, int col) {
		ExerciseManagementController *controller = (ExerciseManagementController*)grid.param();
		controller->m_currentActionSelection = row;
		if (row >= 0)
			controller->m_currentAction = (Action*)grid.item(row, 0)->data();
	}


	static void buttonSaveAction_onClick(Button &button) {
		ExerciseManagementController *controller = (ExerciseManagementController*)button.param();
		controller->saveCurrentAction();
	}

	static void buttonCancelAction_onClick(Button &button) {
		ExerciseManagementController *controller = (ExerciseManagementController*)button.param();
		controller->setEditionMode(0);
	}

	static void comboActionType_onChange(uilib::ComboBox &combo, ComboBoxItem *item) {
		ExerciseManagementController *controller = (ExerciseManagementController*)combo.param();
		controller->m_currentActionTypeSelection = combo.selection();
	}

	void ExerciseManagementController::fillActionTypeCombo(cComboBox &combo) {
		combo.appendItem("Posição",Action::Position);
		combo.appendItem("Movimento",Action::Movement);
	}

	void ExerciseManagementController::fillOrderTypeCombo(cComboBox &combo) {
		combo.appendItem("Nova ação",0);
		combo.appendItem("Última ação",1);
		combo.appendItem("Todas as ações",2);
	}

	void ExerciseManagementController::fillBodyPointCombo(cComboBox &combo) {
		combo.appendItem("Cabeça",0);
		combo.appendItem("Ombro",1);
	}

	void ExerciseManagementController::fillSpaceTypeCombo(cComboBox &combo) {
		combo.appendItem("Mundo", 0);
		combo.appendItem("Última posição", 1);
		combo.appendItem("Cabeça", 1);
	}

	void ExerciseManagementController::fillMovementTypeCombo(cComboBox &combo) {
		combo.appendItem("Linear",0);
		combo.appendItem("Angular",1);
	}

	bool ExerciseManagementController::validateFields() {
		bool required = false;
		bool invalid = false;

		if (cbActionType.selection() == -1
			|| cbOrderType.selection() == -1
			|| cbRefPoint.selection() == -1
			|| tbPositionX.text() == ""
			|| tbPositionY.text() == ""
			|| tbPositionZ.text() == ""
			) {
			required = true;
		}

		if (cbActionType.selection() == 1) {
			if (cbMovementType.selection() == -1) {
				required = true;
			}
		}

		if (required) {
			Message::warning(NULL, "Os campos com '*' devem ser preenchidos obrigatoriamente.");
		}
		else if (invalid) {
			//todo: campos somente numéricos
		}

		return !required && !invalid;
	}

	void ExerciseManagementController::saveCurrentAction() {

		if(!validateFields())
			return;

		string &name = tbName.text();
		int order = cbOrderType.selection();
		int type = cbActionType.selection();
		string &minTimeStr = tbMinTime.text();
		string &maxTimeStr = tbMaxTime.text();
		int bodyPoint = cbBodyPoint.selection();
		int refPoint = cbRefPoint.selection();
		string &posXStr = tbPositionX.text();
		string &posYStr = tbPositionY.text();
		string &posZStr = tbPositionZ.text();
		/*
		string &orientationXStr = editOrientationX.text();
		string &orientationYStr = editOrientationY.text();
		string &orientationZStr = editOrientationZ.text();
		*/

		Action *action;

		if ((Action::ActionType)type == Action::Position) {
			string &minHoldTimeStr = tbMinHoldTime.text();

			PositionAction *posAction;
			if (m_editMode == 1)
				posAction = new PositionAction(*m_currentExercise);
			else
				posAction = (PositionAction*)m_currentAction;
			posAction->setMinHoldTime(string_op::decimal(minHoldTimeStr.data()));

			action = posAction;
		}
		else if ((Action::ActionType)type == Action::Movement) {
			int movementType = cbMovementType.selection();
			string &minSpeedStr = tbMinSpeed.text();
			string &maxSpeedStr = tbMaxSpeed.text();

			MovementAction *movementAction;
			if (m_editMode == 1)
				movementAction = new MovementAction(*m_currentExercise);
			else
				movementAction = (MovementAction*)m_currentAction;
			movementAction->setMovementType((MovementAction::MovementType)movementType);
			movementAction->setMinSpeed(string_op::decimal(minSpeedStr.data()));
			movementAction->setMaxSpeed(string_op::decimal(maxSpeedStr.data()));

			action = movementAction;
		}
		else {
			//Should never reach here
			return;
		}

		action->setName(name.data());
		action->setOrder((Action::ActionOrder)order);
		action->setMinTime(string_op::decimal(minTimeStr.data()));
		action->setMaxTime(string_op::decimal(maxTimeStr.data()));
		action->setBodyPoint((BodyPoint::BodyPart)bodyPoint);
		action->setRefPoint(refPoint);
		action->setPosition(Vector3(string_op::decimal(posXStr.data()), string_op::decimal(posYStr.data()), string_op::decimal(posZStr.data())));

		

		if (m_editMode == 1) {
			g_cinetico.cineticoDB()->actionDAO()->create(*action);
			delete action;
		}
		else {
			g_cinetico.cineticoDB()->actionDAO()->update(*action);
		}

		setEditionMode(0);
	}

	ExerciseManagementController::ExerciseManagementController()
		: title("Gerenciamento de Ações", "Adicione ou altere ações para o exercício.")
	{
		buttonAdd.setText("Adicionar");
		buttonAdd.setParam(this);
		buttonAdd.setOnClick(buttonAdd_onClick);
		buttonBack.setText("Voltar");
		buttonBack.setParam(this);
		buttonBack.setOnClick(buttonBack_onClick);
		layoutActionButtons.append(buttonAdd);
		layoutActionButtons.append(buttonBack);

		//List
		gridActions.setStyle(CS_Border);
		gridActions.setColumnCount(3);
		gridActions.setHeaderVisible(true);
		gridActions.setHeaderText(0, "Tipo");
		gridActions.setHeaderText(1, "Nome");
		gridActions.setHeaderText(2, "Posição (cm)");
		gridActions.setParam(this);
		gridActions.setOnItemSelect(gridActions_onChange);
		buttonEdit.setText("Editar");
		buttonEdit.setParam(this);
		buttonEdit.setOnClick(buttonEdit_onClick);
		buttonDelete.setText("Excluir");
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
		buttonSaveAction.setText("Salvar");
		buttonSaveAction.setParam(this);
		buttonSaveAction.setOnClick(buttonSaveAction_onClick);
		buttonCancelAction.setText("Cancelar");
		buttonCancelAction.setParam(this);
		buttonCancelAction.setOnClick(buttonCancelAction_onClick);
		layoutActionDataActionButtons.append(buttonSaveAction);
		layoutActionDataActionButtons.append(buttonCancelAction);

		//Base Action data
		separatorActionBasicData.setText("Informação básica da ação");
		cbActionType.setLabel("Tipo *");
		fillActionTypeCombo(cbActionType);
		cbActionType.combo.setParam(this);
		cbActionType.combo.setOnSelect(comboActionType_onChange);

		cbOrderType.setLabel("Ordem *");
		fillOrderTypeCombo(cbOrderType);

		tbName.setLabel("Nome");

		layoutActionDataRow1.append(cbActionType, Size(SizeTypeMax, SizeTypeAuto));
		layoutActionDataRow1.append(cbOrderType, Size(SizeTypeMax, SizeTypeAuto));
		layoutActionDataRow1.append(tbName);


		cbBodyPoint.setLabel("Ponto do corpo *");
		fillBodyPointCombo(cbBodyPoint);
		cbRefPoint.setLabel("Ponto de referência *");
		fillSpaceTypeCombo(cbRefPoint);

		tbMinTime.setLabel("Tempo mínimo para execução (s)");
		tbMaxTime.setLabel("Tempo máximo para execução (s)");

		layoutBaseActionData.append(cbBodyPoint, Size(SizeTypeMax, SizeTypeAuto));
		layoutBaseActionData.append(cbRefPoint, Size(SizeTypeMax, SizeTypeAuto));
		layoutBaseActionData.append(tbMinTime, Size(SizeTypeMax, SizeTypeAuto));
		layoutBaseActionData.append(tbMaxTime, Size(SizeTypeMax, SizeTypeAuto));


		labelPosition.setText("Posição *");
		tbPositionX.setLabel("X");
		tbPositionY.setLabel("Y");
		tbPositionZ.setLabel("Z");
		layoutPosition.append(tbPositionX);
		layoutPosition.append(tbPositionY);
		layoutPosition.append(tbPositionZ);


		//Position Action
		tbMinHoldTime.setLabel("Tempo mínimo para segurar a posição (s)");
		layoutPositionSpecific.append(tbMinHoldTime);

		//Movement Action
		cbMovementType.setLabel("Tipo de movimento *");
		fillMovementTypeCombo(cbMovementType);

		tbMinSpeed.setLabel("Velocidade mínima para execução (cm/s²)");
		tbMaxSpeed.setLabel("Velocidade máxima para execução (cm/s²)");

		layoutMovementAction.append(cbMovementType, Size(SizeTypeMax, SizeTypeAuto));
		layoutMovementAction.append(tbMinSpeed, Size(SizeTypeMax, SizeTypeAuto));
		layoutMovementAction.append(tbMaxSpeed, Size(SizeTypeMax, SizeTypeAuto));
		layoutMovementSpecific.append(layoutMovementAction);

		separatorSpecificData.setText("Informação específica para o tipo de ação");
		layoutSpecific.append(separatorSpecificData);

		layoutActionData.append(layoutActionDataActionButtons, AutoSize, 10);
		layoutActionData.append(separatorActionBasicData);
		layoutActionData.append(layoutActionDataRow1);
		layoutActionData.append(layoutBaseActionData);
		layoutActionData.append(labelPosition);
		layoutActionData.append(layoutPosition,AutoSize,20);
		layoutActionData.append(layoutSpecific);
		//
		layoutContent.append(layoutContentList);

		layout.append(title);
		layout.append(layoutActionButtons);
		layout.append(separatorContent);
		layout.append(layoutContent);
		layout.setMargin(10);

	}

	Layout *ExerciseManagementController::viewDefinition() {
		return &layout; 
	}

	void ExerciseManagementController::onViewEnter(ViewParams *params) {
		m_editMode = 0;
		m_currentActionTypeSelection = -1;
		m_currentMovementTypeSelection = -1;
		buttonEdit.setEnabled(false);

		Exercise *exercise = params ? (Exercise*)(*params)["exercise"] : NULL;
		m_currentExercise = exercise;
		updateActionList();
	}

	void ExerciseManagementController::onViewTick() {
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

			layout.setSize(layout.size());
			layout.setVisible(true);
		}

		lastEditMode = m_editMode;
		lastActionSelection = m_currentActionSelection;
		lastActionType = m_currentActionTypeSelection;
	}

	void ExerciseManagementController::onViewQuit() {

	}

	void ExerciseManagementController::setEditionMode(int mode) {
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
			cbActionType.setSelection(-1);
			cbActionType.setEnabled(mode == 1);
			cbOrderType.setSelection(-1);
			tbName.setText("");
			cbBodyPoint.setSelection(-1);
			cbRefPoint.setSelection(-1);
			tbMinTime.setText("");
			tbMaxTime.setText("");
			tbPositionX.setText("");
			tbPositionY.setText("");
			tbPositionZ.setText("");
			//
			tbMinHoldTime.setText("");
			//
			cbMovementType.setSelection(-1);
			tbMinSpeed.setText("");
			tbMaxSpeed.setText("");
			if (mode == 2) {
				Action *action = m_currentAction;
				cbActionType.setSelection(action->type());
				cbOrderType.setSelection(action->order());
				tbName.setText(action->name().c_str());
				cbBodyPoint.setSelection(action->bodyPoint());
				cbRefPoint.setSelection(action->refPoint());
				tbMinTime.setText(string::fromFloat(action->minTime()).data());
				tbMaxTime.setText(string::fromFloat(action->maxTime()).data());
				tbPositionX.setText(string::fromFloat(action->position().x()).data());
				tbPositionY.setText(string::fromFloat(action->position().y()).data());
				tbPositionZ.setText(string::fromFloat(action->position().z()).data());

				if (action->type() == Action::Position) {
					tbMinHoldTime.setText(string::fromFloat(((PositionAction*)action)->minHoldTime()).data());
				}
				else if (action->type() == Action::Movement) {
					cbMovementType.setSelection(((MovementAction*)action)->movementType());
					tbMinSpeed.setText(string::fromFloat(((MovementAction*)action)->minSpeed()).data());
					tbMinSpeed.setText(string::fromFloat(((MovementAction*)action)->maxSpeed()).data());
				}

			}
		}

		m_editMode = mode;
		layout.setSize(layout.size());
		layout.setVisible(true);
	}

	void ExerciseManagementController::updateActionList() {
		std::vector<Action*> actionList = g_cinetico.cineticoDB()->actionDAO()->getAllActionsByExercise(*m_currentExercise);
		gridActions.deleteRows();
		for (unsigned int i = 0; i < actionList.size(); ++i) {
			Action *action = actionList[i];
			gridActions.insertRow();
			int lastRow = gridActions.rowCount() - 1;
			gridActions.setItem(lastRow, 0, new ListViewItem(action->type() == Action::Position ? "Posição" : "Movimento", Color(0, 0, 0), FontDesc("Arial", 10, 0), action));
			gridActions.setItem(lastRow, 1, new ListViewItem(action->name().c_str()));

			std::ostringstream ss;
			ss << "[";
			ss << action->position().x();
			ss << ", ";
			ss << action->position().y();
			ss << ", ";
			ss << action->position().z();
			ss << "]";

			gridActions.setItem(lastRow, 2, new ListViewItem(ss.str().c_str()));
		}
		m_currentActionSelection = -1;
		m_currentAction = NULL;
	}

}