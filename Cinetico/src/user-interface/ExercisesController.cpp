
#include "cinetico.h"
#include "ExercisesController.h"

namespace cinetico {

	using namespace cinetico_core;
	extern Cinetico g_cinetico;

	static void buttonCreateNew_onClick(Button &button) {
		ExercisesController *controller = (ExercisesController*)button.param();
		controller->setEditionMode(1);
	}

	static void buttonManageActions_onClick(Button &button) {
		ExercisesController *controller = (ExercisesController*)button.param();
		if (controller->m_currentExercise) {
			Controller::ViewParams params;
			params["exercise"] = controller->m_currentExercise;
			g_cinetico.goTo(Cinetico::EXERCISE_MANAGEMENT, &params);
		}
	}

	static void buttonDoExercise_onClick(Button &button) {
		ExercisesController *controller = (ExercisesController*)button.param();

		Exercise *exercise = (Exercise*)controller->gridExercises.item(0, 0)->data();
		controller->doSelectedExercise();
	}

	static void onClick_CreateEdit(Button &button) {
		ExercisesController *controller = (ExercisesController*)button.param();
		controller->saveCurrentExercise();
	}

	static void onClick_Cancel(Button &button) {
		ExercisesController *controller = (ExercisesController*)button.param();
		controller->setEditionMode(0);
	}

	static void gridExercises_onItemSelect(GridView& grid, int row, int col) {
		ExercisesController *controller = (ExercisesController*)grid.param();
		controller->onExerciseSelect(row);
	}

	static void buttonEdit_onClick(Button& button) {
		ExercisesController *controller = (ExercisesController*)button.param();
		controller->setEditionMode(2);
	}

	static void buttonDelete_onClick(Button& button) {
		ExercisesController *controller = (ExercisesController*)button.param();
		controller->deleteSelectedExercise();
	}

	void ExercisesController::setEditionMode(int mode) {

		if (mode == m_editMode)
			return;

		if(m_editMode == 0) {
			layoutContent.remove(layoutContentList);
		}
		else if (m_editMode == 1) {
			layoutContent.remove(layoutContentEditData);
		}
		else if (m_editMode == 2) {
			layoutContent.remove(layoutContentEditData);
		}

		m_editMode = mode;

		if (mode == 0) {
			layoutContent.append(layoutContentList);
			updateExerciseList();
		}
		else if (mode == 1) {
			editExerciseName.setEditable(true);
			editExerciseName.setText("");
			checkPublic.setChecked(false);
			for (int i = 0; i < checkBodyPointList.size(); ++i) {
				checkBodyPointList[i].setChecked(true);
			}
			layoutContent.append(layoutContentEditData);
		}
		else if (mode == 2) {
			if (!m_currentExercise)
				return;

			editExerciseName.setEditable(true);
			editExerciseName.setText(m_currentExercise->name().c_str());
			checkPublic.setChecked(m_currentExercise->isPublic());
			int trackableBodyPoints = m_currentExercise->trackableBodyPoints();
			for (int i = 0; i < m_checkBodyPointValueList.size(); ++i) {
				int value = m_checkBodyPointValueList[i];
				checkBodyPointList[i].setChecked((trackableBodyPoints&value) != 0);
			}
			layoutContent.append(layoutContentEditData);
		}

		layoutContent.setSize(layoutContent.size());
		layoutContent.setVisible(true);
	}

	void ExercisesController::onExerciseSelect(int row) {
		m_currentSelection = row;
		if (row >= 0)
			m_currentExercise = (Exercise*)gridExercises.item(row, 0)->data();
	}

	void ExercisesController::updateExerciseList() {
		std::vector<Exercise*> exerciseList = g_cinetico.cineticoDB()->exerciseDAO()->getAllExercisesByUserProfile(g_cinetico.currentUser());
		gridExercises.deleteRows();
		for (unsigned int i = 0; i < exerciseList.size(); ++i) {
			Exercise *exercise = exerciseList[i];
			gridExercises.insertRow();
			int lastRow = gridExercises.rowCount() - 1;

			int actionCount = g_cinetico.cineticoDB()->actionDAO()->getActionCountByExercise(*exercise);

			gridExercises.setItem(lastRow, 0, new ListViewItem(exercise->name().c_str(), Color(0, 0, 0), FontDesc("Arial", 10, 0), exerciseList[i]));
			gridExercises.setItem(lastRow, 1, new ListViewItem(exercise->author().c_str()));
			gridExercises.setItem(lastRow, 2, new ListViewItem(uilib::string::fromInteger(actionCount).data()));
			gridExercises.setItem(lastRow, 3, new ListViewItem(exercise->isPublic() ? "Sim" : "Não"));
		}
		m_currentSelection = -1;
		m_currentExercise = NULL;
	}

	ExercisesController::ExercisesController() {

		labelViewTitle.setText("Exercícios");
		labelViewTitle.setFont(FontDesc("Arial", 46, FONT_BOLD));
		labelViewDescr.setText("Crie, altere, grave ou realize um exercício.");
		labelViewDescr.setFont(FontDesc("Arial", 10, FONT_BOLD));

		buttonCreateExercise.setText("Criar Exercício");
		buttonCreateExercise.setParam(this);
		buttonCreateExercise.setOnClick(buttonCreateNew_onClick);
		buttonManageActions.setText("Gerenciar ações");
		buttonManageActions.setParam(this);
		buttonManageActions.setOnClick(buttonManageActions_onClick);
		buttonDoExercise.setText("Realizar Exercício");
		buttonDoExercise.setParam(this);
		buttonDoExercise.setOnClick(buttonDoExercise_onClick);

		gridExercises.setParam(this);
		gridExercises.setStyle(CS_Border);
		gridExercises.setColumnCount(4);
		gridExercises.setHeaderVisible(true);
		gridExercises.setHeaderText(0, "Nome");
		gridExercises.setHeaderText(1, "Autor");
		gridExercises.setHeaderText(2, "Número de ações");
		gridExercises.setHeaderText(3, "Público");
		gridExercises.setOnItemSelect(gridExercises_onItemSelect);

		buttonEdit.setText("Editar");
		buttonEdit.setParam(this);
		buttonEdit.setOnClick(buttonEdit_onClick);
		buttonDelete.setText("Excluir");
		buttonDelete.setParam(this);
		buttonDelete.setOnClick(buttonDelete_onClick);

		layoutActionButtons.append(buttonCreateExercise);
		layoutActionButtons.append(buttonManageActions);
		layoutActionButtons.append(buttonDoExercise);

		layoutActions.append(labelViewTitle);
		layoutActions.append(labelViewDescr);
		layoutActions.append(separatorActionButtons);
		layoutActions.append(layoutActionButtons);

		layoutExerciseListActions.append(buttonEdit);
		layoutExerciseListActions.append(buttonDelete);

		layoutContentList.setAlignment(Layout::center_align);
		layoutContentList.append(gridExercises, MaximumSize);
		layoutContentList.append(layoutExerciseListActions);
		layoutContent.append(layoutContentList);

		labelExerciseName.setText("Nome do exercício");
		checkPublic.setText("Público");
		separatorChecks.setText("Partes do corpo que serão utilizadas");
		separatorChecks.setFont(FontDesc("Arial", 12, FONT_BOLD));

		/*Preenchendo os layouts e checkBoxes do corpo*/
		std::vector<BodyPointConfig*> bpConfig = g_cinetico.getAllBodyPointsCaps();
		size_t bpConfigCount = bpConfig.size();
		checkBodyPointList.resize(bpConfigCount);
		m_checkBodyPointValueList.resize(bpConfigCount);
		int bpLayoutCount = bpConfigCount / 3;
		if (bpConfigCount % 3 != 0)
			bpLayoutCount += 1;
		layoutCheckBodyPointList.resize(bpLayoutCount);

		size_t i, j = 0, k = 0;;
		for (i = 0; i < bpConfigCount / 3; ++i) {
			for (j = 0; j < 3; ++j) {
				checkBodyPointList[k].setText(bpConfig[k]->name);
				m_checkBodyPointValueList[k] = bpConfig[k]->value;
				layoutCheckBodyPointList[i].append(checkBodyPointList[k], Size(SizeTypeMax, SizeTypeAuto));
				++k;
			}
		}

		for (; k < bpConfigCount; ++k) {
			checkBodyPointList[k].setText(bpConfig[k]->name);
			m_checkBodyPointValueList[k] = bpConfig[k]->value;
			layoutCheckBodyPointList[i].append(checkBodyPointList[k], Size(SizeTypeMax, SizeTypeAuto));
		}

		buttonCreateEdit.setText("Salvar");
		buttonCreateEdit.setParam(this);
		buttonCreateEdit.setOnClick(onClick_CreateEdit);
		buttonCancel.setText("Cancelar");
		buttonCancel.setParam(this);
		buttonCancel.setOnClick(onClick_Cancel);

		layoutEditActions.append(buttonCreateEdit);
		layoutEditActions.append(buttonCancel);

		layoutExerciseNameData.append(editExerciseName);
		layoutExerciseNameData.append(checkPublic);
		layoutExerciseNameData.setAlignment(Layout::center_align);
		layoutExerciseName.append(labelExerciseName);
		layoutExerciseName.append(layoutExerciseNameData);

		layoutExerciseData.append(layoutExerciseName, AutoSize, 20);
		layoutExerciseData.append(separatorChecks);
		for (i = 0; i < bpLayoutCount; ++i)
			layoutExerciseData.append(layoutCheckBodyPointList[i]);

		layoutContentEditData.append(layoutExerciseData, AutoSize, 20);
		layoutContentEditData.append(layoutEditActions);

		layout.append(layoutActions,AutoSize,20);
		layout.append(layoutContent);
		layout.setMargin(10);
	}

	Layout *ExercisesController::viewDefinition() {
		return &layout;
	}

	void ExercisesController::onViewEnter(ViewParams *params) {
		m_editMode = 0;
		m_currentSelection = -1;
		buttonCreateExercise.setEnabled(true);
		buttonEdit.setEnabled(false);
		buttonDelete.setEnabled(false);
		buttonManageActions.setEnabled(false);
		buttonDoExercise.setEnabled(false);
		updateExerciseList();
	}

	void ExercisesController::onViewTick() {
		static int lastEditMode = 0;
		static int lastSelection = -1;
		if (m_editMode != lastEditMode) {
			buttonCreateExercise.setEnabled(m_editMode == 0);
			buttonManageActions.setEnabled(m_editMode == 0);
			buttonDoExercise.setEnabled(m_editMode == 0 && m_currentSelection >= 0);
		}

		if (m_currentSelection != lastSelection) {
			buttonEdit.setEnabled(m_currentSelection >= 0);
			buttonDelete.setEnabled(m_currentSelection >= 0);
			buttonManageActions.setEnabled(m_currentSelection >= 0);
			buttonDoExercise.setEnabled(m_editMode == 0 && m_currentSelection >= 0);
		}

		lastEditMode = m_editMode;
		lastSelection = m_currentSelection;

	}

	void ExercisesController::onViewQuit() {

	}


	void ExercisesController::saveCurrentExercise() {

		std::string exerciseName = editExerciseName.text().data();
		if (exerciseName == "")
			return;

		std::string authorName = g_cinetico.currentUser()->username();
		unsigned long bpFlags = 0;
		bool isPublic = checkPublic.checked();
		for (size_t i = 0; i < checkBodyPointList.size(); ++i) {
			if (checkBodyPointList[i].checked())
				bpFlags |= m_checkBodyPointValueList[i];
		}

		//editAuthorName.setText(authorName);

		Exercise *exercise;
		if (m_editMode == 1) {
			exercise = new Exercise();
			exercise->setName(exerciseName.c_str());
			exercise->setAuthor(authorName.c_str());
			exercise->setPublic(isPublic);
			exercise->setTrackableBodyPoints(bpFlags);

			g_cinetico.cineticoDB()->exerciseDAO()->create(*exercise, g_cinetico.currentUser());
			delete exercise;
		}
		else if (m_editMode == 2) {
			exercise = m_currentExercise;
			exercise->setName(exerciseName.c_str());
			exercise->setAuthor(authorName.c_str());
			exercise->setPublic(isPublic);
			exercise->setTrackableBodyPoints(bpFlags);
			g_cinetico.cineticoDB()->exerciseDAO()->update(*exercise);
		}

		setEditionMode(0);
	}

	void ExercisesController::deleteSelectedExercise() {
		if (m_currentExercise) {
			Message::message_result res = Message::question(NULL, "Deseja excluir este registro?");
			if (res == Message::yes) {
				g_cinetico.cineticoDB()->exerciseDAO()->exclude(*m_currentExercise);
				updateExerciseList();
			}
		}
	}

	void ExercisesController::doSelectedExercise() {
		if (m_currentExercise) {

			std::vector<Action*> actionList = g_cinetico.cineticoDB()->actionDAO()->getAllActionsByExercise(*m_currentExercise);

			std::string str;
			if (actionList.size() == 0) {
				Message::warning(NULL, "Para realizar um exercício, este deve conter pelo menos uma ação.");
				return;
			}

			m_currentExercise->setActionList(actionList);

			Message::msg(NULL, "Congratulations. Now get into the 3D World.");
			ViewParams params;
			params["exercise"] = m_currentExercise;
			g_cinetico.goTo(Cinetico::EXERCISE_REALIZATION,&params);
		}
	}

}