
#include "cinetico.h"
#include "ExerciseManagementController.h"

#include "entity/core/PositionAction.h"
#include "entity/core/MovementAction.h"

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
		combo.fastinsertItem("Cabe�a");
		combo.fastinsertItem("Ombro");
	}

	void ExerciseManagementController::fillSpaceTypeCombo(ComboBox &combo) {
		combo.fastinsertItem("Mundo");
		combo.fastinsertItem("�ltima posi��o");
		combo.fastinsertItem("Cabe�a");
	}

	bool ExerciseManagementController::checkRequiredFields() {

	}

	void ExerciseManagementController::saveCurrentAction() {

		if (!checkRequiredFields())
			return;

		string &name = editName.text();
		int order = comboPartOf.selection();
		int type = comboActionType.selection();
		string &minTimeStr = editMinTime.text();
		string &maxTimeStr = editMaxTime.text();
		int bodyPoint = comboBodyPoint.selection();
		int refPoint = comboRefPoint.selection();
		string &posXStr = editPositionX.text();
		string &posYStr = editPositionX.text();
		string &posZStr = editPositionX.text();

		Action *action;

		if ((Action::ActionType)type == Action::Position) {
			string &minHoldTimeStr = editMinHoldtime.text();

			PositionAction *posAction = new PositionAction();
			posAction->setMinHoldTime(string_op::decimal(minHoldTimeStr.data()));

			action = posAction;
		}
		else if ((Action::ActionType)type == Action::Movement) {
			int movementType = comboMovementType.selection();
			string &minSpeedStr = editMinSpeed.text();
			string &maxSpeedStr = editMaxSpeed.text();

			MovementAction *movementAction = new MovementAction();
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

		g_cinetico.cineticoDB().actionDAO().save(action);
	}

	ExerciseManagementController::ExerciseManagementController() {
		labelViewTitle.setText("Gerenciamento de Exerc�cio");
		labelViewTitle.setFont(FontDesc("Arial", 46, FONT_BOLD));
		labelViewDescr.setText("Adicione ou altere a��es para um exerc�cio.");
		labelViewDescr.setFont(FontDesc("Arial", 10, FONT_BOLD));

		buttonBack.setText("Voltar");
		buttonBack.setParam(this);
		buttonBack.setOnClick(buttonBack_onClick);
		layoutActionButtons.append(buttonBack);
		layoutActions.append(labelViewTitle);
		layoutActions.append(labelViewDescr);
		layoutActions.append(layoutActionButtons);

		//List
		gridActions.setStyle(CS_Border);
		gridActions.setColumnCount(3);
		gridActions.setHeaderVisible(true);
		gridActions.setHeaderText(0, "Tipo *");
		gridActions.setHeaderText(1, "Nome");
		gridActions.setHeaderText(2, "Posi��o");
		buttonAdd.setText("Adicionar");
		buttonAdd.setParam(this);
		buttonAdd.setOnClick(buttonAdd_onClick);
		buttonEdit.setText("Editar");
		buttonDelete.setText("Excluir");

		layoutGridActions.append(buttonAdd);
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
		labelName.setText("Nome");
		layoutName.append(labelName);
		layoutName.append(editName);

		labelPartOf.setText("Ordem *");
		comboPartOf.fastinsertItem("Nova a��o");
		comboPartOf.fastinsertItem("�ltima a��o");
		comboPartOf.fastinsertItem("Todas as a��es");
		layoutPartOf.append(labelPartOf);
		layoutPartOf.append(comboPartOf, Size(SizeTypeMax, SizeTypeAuto));

		layoutActionDataRow1.append(layoutName);
		layoutActionDataRow1.append(layoutPartOf, Size(MakePercentType(30), SizeTypeAuto));

		labelActionType.setText("Tipo");
		comboActionType.fastinsertItem("Posi��o");
		comboActionType.fastinsertItem("Movimento");
		comboActionType.setParam(this);
		comboActionType.setOnSelect(comboActionType_onChange);
		layoutActionType.append(labelActionType);
		layoutActionType.append(comboActionType, Size(SizeTypeMax, SizeTypeAuto));

		labelMinTime.setText("Tempo m�nimo para execu��o (s)");
		layoutMinTime.append(labelMinTime);
		layoutMinTime.append(editMinTime);

		labelMaxTime.setText("Tempo m�ximo para execu��o (s)");
		layoutMaxTime.append(labelMaxTime);
		layoutMaxTime.append(editMaxTime);

		labelBodyPoint.setText("Ponto do corpo *");
		labelRefPoint.setText("Ponto de refer�ncia *");
		fillBodyPointCombo(comboBodyPoint);
		fillSpaceTypeCombo(comboRefPoint);
		layoutBodyAndRefPoints.append(layoutBodyPoint);
		layoutBodyAndRefPoints.append(layoutRefPoint);

		labelPosition.setText("Posi��o *");
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


		layoutBaseActionData.append(layoutActionType,Size(SizeTypeMax, SizeTypeAuto));
		layoutBaseActionData.append(layoutMinTime, Size(SizeTypeMax, SizeTypeAuto));
		layoutBaseActionData.append(layoutMaxTime, Size(SizeTypeMax, SizeTypeAuto));


		//Position Action
		labelMinHoldtime.setText("Tempo m�nimo para segurar a posi��o (s)");
		layoutMinHoldTime.append(labelMinHoldtime);
		layoutMinHoldTime.append(editMinHoldtime);
		layoutPositionSpecific.append(layoutMinHoldTime);

		//Movement Action
		labelMovementType.setText("Tipo de movimento");
		comboMovementType.fastinsertItem("Linear");
		comboMovementType.fastinsertItem("Angular");
		layoutMovementType.append(labelMovementType);
		layoutMovementType.append(comboMovementType, Size(SizeTypeMax, SizeTypeAuto));

		labelMinSpeed.setText("Velocidade m�nima para execu��o (cm/s�)");
		layoutMinSpeed.append(labelMinSpeed);
		layoutMinSpeed.append(editMinSpeed);

		labelMaxSpeed.setText("Velocidade m�xima para execu��o (cm/s�)");
		layoutMaxSpeed.append(labelMaxSpeed);
		layoutMaxSpeed.append(editMaxSpeed);

		layoutMovementAction.append(layoutMovementType, Size(SizeTypeMax, SizeTypeAuto));
		layoutMovementAction.append(layoutMinSpeed, Size(SizeTypeMax, SizeTypeAuto));
		layoutMovementAction.append(layoutMaxSpeed, Size(SizeTypeMax, SizeTypeAuto));
		layoutMovementSpecific.append(layoutMovementAction);

		layoutActionData.append(layoutActionDataActionButtons);
		layoutActionData.append(layoutActionDataRow1);
		layoutActionData.append(layoutBaseActionData);
		layoutActionData.append(labelSpaceType);
		layoutActionData.append(layoutSpaceType);
		layoutActionData.append(labelPosition);
		layoutActionData.append(layoutPosition);
		layoutActionData.append(layoutSpecific);
		//
		layoutContent.append(layoutContentList);

		layout.append(layoutActions, AutoSize, 20);
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
		Exercise *exercise = params ? (Exercise*)(*params)["exercise"] : NULL;
		m_currentExercise = exercise;
	}

	void ExerciseManagementController::onViewTick() {
		static int lastActionType = -1;
		static int lastMovementType = -1;

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

		lastActionType = m_currentActionTypeSelection;
	}

	void ExerciseManagementController::onViewQuit() {

	}

	void ExerciseManagementController::setEditionMode(int mode) {
		if (m_editMode == 0)
			layoutContent.remove(layoutContentList);
		else if (m_editMode == 1) {
			layoutContent.remove(layoutActionData);
		}

		if (mode == 0) {
			layoutContent.append(layoutContentList);
		}
		else if (mode == 1) {
			layoutContent.append(layoutActionData);
		}

		m_editMode = mode;
		layout.setSize(layout.size());
		layout.setVisible(true);
	}
}