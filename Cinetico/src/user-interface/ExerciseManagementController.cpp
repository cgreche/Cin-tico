
#include "cinetico.h"
#include "ExerciseManagementController.h"

#include "entity/core/PositionAction.h"
#include "entity/core/MovementAction.h"

#include <sstream>

namespace cinetico {
	using namespace cinetico_core;
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

	static void comboActionType_onChange(ComboBox &combo, ComboBoxItem *item) {
		ExerciseManagementController *controller = (ExerciseManagementController*)combo.param();
		controller->m_currentActionTypeSelection = combo.selection();
	}

	void ExerciseManagementController::fillBodyPointCombo(ComboBox &combo) {
		combo.fastinsertItem("Cabeça");
		combo.fastinsertItem("Ombro");
	}

	void ExerciseManagementController::fillSpaceTypeCombo(ComboBox &combo) {
		combo.fastinsertItem("Mundo");
		combo.fastinsertItem("Última posição");
		combo.fastinsertItem("Cabeça");
	}

	bool ExerciseManagementController::validateFields() {
		bool required = false;
		bool invalid = false;

		if (comboActionType.selection() == -1
			|| comboPartOf.selection() == -1
			|| comboRefPoint.selection() == -1
			|| editPositionX.text() == ""
			|| editPositionY.text() == ""
			|| editPositionZ.text() == ""
			) {
			required = true;
		}

		if (comboActionType.selection() == 1) {
			if (comboMovementType.selection() == -1) {
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

		string &name = editName.text();
		int order = comboPartOf.selection();
		int type = comboActionType.selection();
		string &minTimeStr = editMinTime.text();
		string &maxTimeStr = editMaxTime.text();
		int bodyPoint = comboBodyPoint.selection();
		int refPoint = comboRefPoint.selection();
		string &posXStr = editPositionX.text();
		string &posYStr = editPositionY.text();
		string &posZStr = editPositionZ.text();
		/*
		string &orientationXStr = editOrientationX.text();
		string &orientationYStr = editOrientationY.text();
		string &orientationZStr = editOrientationZ.text();
		*/

		Action *action;

		if ((Action::ActionType)type == Action::Position) {
			string &minHoldTimeStr = editMinHoldtime.text();

			PositionAction *posAction;
			if (m_editMode == 1)
				posAction = new PositionAction(*m_currentExercise);
			else
				posAction = (PositionAction*)m_currentAction;
			posAction->setMinHoldTime(string_op::decimal(minHoldTimeStr.data()));

			action = posAction;
		}
		else if ((Action::ActionType)type == Action::Movement) {
			int movementType = comboMovementType.selection();
			string &minSpeedStr = editMinSpeed.text();
			string &maxSpeedStr = editMaxSpeed.text();

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

	ExerciseManagementController::ExerciseManagementController() {
		labelViewTitle.setText("Gerenciamento de Exercício");
		labelViewTitle.setFont(FontDesc("Arial", 46, FONT_BOLD));
		labelViewDescr.setText("Adicione ou altere ações para um exercício.");
		labelViewDescr.setFont(FontDesc("Arial", 10, FONT_BOLD));

		buttonAdd.setText("Adicionar");
		buttonAdd.setParam(this);
		buttonAdd.setOnClick(buttonAdd_onClick);
		buttonBack.setText("Voltar");
		buttonBack.setParam(this);
		buttonBack.setOnClick(buttonBack_onClick);
		layoutActionButtons.append(buttonAdd);
		layoutActionButtons.append(buttonBack);
		layoutActions.append(labelViewTitle);
		layoutActions.append(labelViewDescr);
		layoutActions.append(separatorActionButtons);
		layoutActions.append(layoutActionButtons);
		layoutActions.append(separatorContent);

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
		separatorActionBasicData.setFont(FontDesc("Arial",12,FONT_BOLD));
		labelActionType.setText("Tipo *");
		comboActionType.fastinsertItem("Posição");
		comboActionType.fastinsertItem("Movimento");
		comboActionType.setParam(this);
		comboActionType.setOnSelect(comboActionType_onChange);
		layoutActionType.append(labelActionType);
		layoutActionType.append(comboActionType, Size(SizeTypeMax, SizeTypeAuto));
		labelPartOf.setText("Ordem *");
		comboPartOf.fastinsertItem("Nova ação");
		comboPartOf.fastinsertItem("Última ação");
		comboPartOf.fastinsertItem("Todas as ações");
		layoutPartOf.append(labelPartOf);
		layoutPartOf.append(comboPartOf, Size(SizeTypeMax, SizeTypeAuto));
		labelName.setText("Nome");
		layoutName.append(labelName);
		layoutName.append(editName);
		layoutActionDataRow1.append(layoutActionType, Size(MakePercentType(30), SizeTypeAuto));
		layoutActionDataRow1.append(layoutPartOf, Size(MakePercentType(30), SizeTypeAuto));
		layoutActionDataRow1.append(layoutName);


		labelBodyPoint.setText("Ponto do corpo *");
		labelRefPoint.setText("Ponto de referência *");
		fillBodyPointCombo(comboBodyPoint);
		fillSpaceTypeCombo(comboRefPoint);
		layoutBodyPoint.append(labelBodyPoint);
		layoutBodyPoint.append(comboBodyPoint, Size(SizeTypeMax, SizeTypeAuto));
		layoutRefPoint.append(labelRefPoint);
		layoutRefPoint.append(comboRefPoint, Size(SizeTypeMax, SizeTypeAuto));

		labelMinTime.setText("Tempo mínimo para execução (s)");
		layoutMinTime.append(labelMinTime);
		layoutMinTime.append(editMinTime);

		labelMaxTime.setText("Tempo máximo para execução (s)");
		layoutMaxTime.append(labelMaxTime);
		layoutMaxTime.append(editMaxTime);

		layoutBaseActionData.append(layoutBodyPoint, Size(SizeTypeMax, SizeTypeAuto));
		layoutBaseActionData.append(layoutRefPoint, Size(SizeTypeMax, SizeTypeAuto));
		layoutBaseActionData.append(layoutMinTime, Size(SizeTypeMax, SizeTypeAuto));
		layoutBaseActionData.append(layoutMaxTime, Size(SizeTypeMax, SizeTypeAuto));


		labelPosition.setText("Posição *");
		labelPositionX.setText("X");
		layoutPositionX.append(labelPositionX);
		layoutPositionX.append(editPositionX);
		labelPositionY.setText("Y");
		layoutPositionY.append(labelPositionY);
		layoutPositionY.append(editPositionY);
		labelPositionZ.setText("Z");
		layoutPositionZ.append(labelPositionZ);
		layoutPositionZ.append(editPositionZ);
		layoutPosition.append(layoutPositionX);
		layoutPosition.append(layoutPositionY);
		layoutPosition.append(layoutPositionZ);




		//Position Action
		labelMinHoldtime.setText("Tempo mínimo para segurar a posição (s)");
		layoutMinHoldTime.append(labelMinHoldtime);
		layoutMinHoldTime.append(editMinHoldtime);
		layoutPositionSpecific.append(layoutMinHoldTime);

		//Movement Action
		labelMovementType.setText("Tipo de movimento *");
		comboMovementType.fastinsertItem("Linear");
		comboMovementType.fastinsertItem("Angular");
		layoutMovementType.append(labelMovementType);
		layoutMovementType.append(comboMovementType, Size(SizeTypeMax, SizeTypeAuto));

		labelMinSpeed.setText("Velocidade mínima para execução (cm/s²)");
		layoutMinSpeed.append(labelMinSpeed);
		layoutMinSpeed.append(editMinSpeed);

		labelMaxSpeed.setText("Velocidade máxima para execução (cm/s²)");
		layoutMaxSpeed.append(labelMaxSpeed);
		layoutMaxSpeed.append(editMaxSpeed);

		layoutMovementAction.append(layoutMovementType, Size(SizeTypeMax, SizeTypeAuto));
		layoutMovementAction.append(layoutMinSpeed, Size(SizeTypeMax, SizeTypeAuto));
		layoutMovementAction.append(layoutMaxSpeed, Size(SizeTypeMax, SizeTypeAuto));
		layoutMovementSpecific.append(layoutMovementAction);

		separatorSpecificData.setText("Informação específica para o tipo de ação");
		separatorSpecificData.setFont(FontDesc("Arial", 12, FONT_BOLD));
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

		layout.append(layoutActions);
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
			comboActionType.setSelection(-1);
			comboActionType.setEnabled(mode == 1);
			comboPartOf.setSelection(-1);
			editName.setText("");
			comboBodyPoint.setSelection(-1);
			comboRefPoint.setSelection(-1);
			editMinTime.setText("");
			editMaxTime.setText("");
			editPositionX.setText("");
			editPositionY.setText("");
			editPositionZ.setText("");
			//
			editMinHoldtime.setText("");
			//
			comboMovementType.setSelection(-1);
			editMinSpeed.setText("");
			editMaxSpeed.setText("");
			if (mode == 2) {
				Action *action = m_currentAction;
				comboActionType.setSelection(action->type());
				comboPartOf.setSelection(action->order());
				editName.setText(action->name().c_str());
				comboBodyPoint.setSelection(action->bodyPoint());
				comboRefPoint.setSelection(action->refPoint());
				editMinTime.setText(string::fromFloat(action->minTime()));
				editMaxTime.setText(string::fromFloat(action->maxTime()));
				editPositionX.setText(string::fromFloat(action->position().x()));
				editPositionY.setText(string::fromFloat(action->position().y()));
				editPositionZ.setText(string::fromFloat(action->position().z()));

				if (action->type() == Action::Position) {
					editMinHoldtime.setText(string::fromFloat(((PositionAction*)action)->minHoldTime()));
				}
				else if (action->type() == Action::Movement) {
					comboMovementType.setSelection(((MovementAction*)action)->movementType());
					editMinSpeed.setText(string::fromFloat(((MovementAction*)action)->minSpeed()));
					editMinSpeed.setText(string::fromFloat(((MovementAction*)action)->maxSpeed()));
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