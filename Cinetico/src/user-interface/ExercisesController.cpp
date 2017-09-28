
#include "cinetico.h"
#include "ExercisesController.h"

namespace cinetico {

	using namespace cinetico_core;
	extern Cinetico g_cinetico;

	static void onClick_CreateNew(Button &button) {
		ExercisesController *controller = (ExercisesController*)button.param();

		controller->editExerciseName.setEditable(true);
		controller->editExerciseName.setText("");
		controller->checkHead.setChecked(true);
		controller->checkLeftHand.setChecked(true);
		controller->checkRightHand.setChecked(true);
		controller->checkHip.setChecked(true);
		controller->checkLeftFoot.setChecked(true);
		controller->checkRightFoot.setChecked(true);

		controller->setEditionMode(1);
	}

	static void onClick_Do(Button &button) {
		ExercisesController *controller = (ExercisesController*)button.param();

		Exercise *exercise = (Exercise*)controller->gridExercises.item(0, 0)->data();

		std::string str;
		str += "Congratulations. Now get into the 3D World.";
		Message::msg(NULL, str.c_str());
		g_cinetico.enter3DWorld();
		g_cinetico.cinetico3D()->startExercise(*exercise);
	}

	static void onClick_CreateEdit(Button &button) {
		ExercisesController *controller = (ExercisesController*)button.param();

		std::string exerciseName = controller->editExerciseName.text().data();
		std::string authorName = g_cinetico.currentUser()->username();
		unsigned long bpFlags = 0;
		bool isPublic = controller->checkPublic.checked();

		if (controller->checkHead.checked())
			bpFlags |= 1;
		if (controller->checkLeftHand.checked())
			bpFlags |= 2;
		if (controller->checkRightHand.checked())
			bpFlags |= 4;
		if (controller->checkHip.checked())
			bpFlags |= 8;
		if (controller->checkLeftFoot.checked())
			bpFlags |= 16;
		if (controller->checkRightFoot.checked())
			bpFlags |= 32;

		if(exerciseName == "")
			return;
		Exercise *exercise;
		if (controller->m_editMode == 1) {
			exercise = new Exercise();
			exercise->setName(exerciseName.c_str());
			exercise->setAuthor(authorName.c_str());
			exercise->setPublic(isPublic);
			exercise->setTrackableBodyPoints(bpFlags);

			g_cinetico.cineticoDB()->exerciseDAO()->save(*exercise);
			delete exercise;
		}
		else if (controller->m_editMode == 2) {
			exercise = new Exercise();
			exercise->setName(exerciseName.c_str());
			exercise->setAuthor(authorName.c_str());
			exercise->setPublic(isPublic);
			exercise->setTrackableBodyPoints(bpFlags);
			g_cinetico.cineticoDB()->exerciseDAO()->update(*exercise);
		}

		controller->setEditionMode(0);
	}

	static void onClick_Cancel(Button &button) {
		ExercisesController *controller = (ExercisesController*)button.param();
		controller->setEditionMode(0);
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
			layoutContent.append(layoutContentEditData);
		}
		else if (mode == 2) {
			layoutContent.append(layoutContentEditData);
		}
		layoutContent.setSize(layoutContent.size());
		layoutContent.setVisible(true);
	}

	static void gridExercises_onItemSelect(GridView& grid, int itemIndex, int subItemIndex) {
		int i = itemIndex;
		int s = subItemIndex;
	}

	void ExercisesController::updateExerciseList() {
		std::vector<Exercise*> exerciseList = g_cinetico.cineticoDB()->exerciseDAO()->getAllExercises();
		gridExercises.clear();
		for (unsigned int i = 0; i < exerciseList.size(); ++i) {
			gridExercises.insertRow();
			int lastRow = gridExercises.rowCount() - 1;
			gridExercises.setItem(lastRow, 0, new ListViewItem(exerciseList[i]->name().c_str(), Color(0, 0, 0), FontDesc("Arial", 10, 0), exerciseList[i]));
			gridExercises.setItem(lastRow, 1, new ListViewItem(exerciseList[i]->author().c_str()));
			gridExercises.setItem(lastRow, 2, new ListViewItem(exerciseList[i]->isPublic() ? "Sim" : "Não"));
		}
	}

	ExercisesController::ExercisesController() {

		labelViewTitle.setText("Exercícios");
		labelViewTitle.setFont(FontDesc("Arial", 46, FONT_BOLD));
		labelViewDescr.setText("Crie, altere, grave ou realize um exercício.");
		labelViewDescr.setFont(FontDesc("Arial", 10, FONT_BOLD));

		buttonCreateExercise.setText("Criar Exercício");
		buttonCreateExercise.setParam(this);
		buttonCreateExercise.setOnClick(onClick_CreateNew);
		buttonRecordExercise.setText("Gravar Exercício");
		buttonRecordExercise.setEnabled(false);
		buttonDoExercise.setText("Realizar Exercício");
		buttonDoExercise.setParam(this);
		buttonDoExercise.setOnClick(onClick_Do);

		gridExercises.setColumnCount(3);
		gridExercises.setHeaderVisible(true);
		gridExercises.setHeaderText(0, "Nome");
		gridExercises.setHeaderText(1, "Autor");
		gridExercises.setHeaderText(2, "Público");
		gridExercises.setOnItemSelect(gridExercises_onItemSelect);

		layoutActionButtons.append(buttonCreateExercise);
		layoutActionButtons.append(buttonRecordExercise);
		layoutActionButtons.append(buttonDoExercise);

		layoutActions.append(labelViewTitle);
		layoutActions.append(labelViewDescr);
		layoutActions.append(layoutActionButtons);

		layoutContentList.append(gridExercises,MaximumSize);
		layoutContent.append(layoutContentList,MaximumSize);



		labelExerciseName.setText("Nome do exercício");
		checkPublic.setText("Público");
		labelChecks.setText("Partes do corpo que serão utilizadas");
		checkHead.setText("Cabeça");
		checkLeftHand.setText("Mão esquerda");
		checkRightHand.setText("Mão direita");
		checkHip.setText("Quadril");
		checkLeftFoot.setText("Pé esquerdo");
		checkRightFoot.setText("Pé direito");

		buttonCreateEdit.setText("Criar");
		buttonCreateEdit.setParam(this);
		buttonCreateEdit.setOnClick(onClick_CreateEdit);
		buttonCancel.setText("Cancelar");
		buttonCancel.setParam(this);
		buttonCancel.setOnClick(onClick_Cancel);

		layoutEditActions.append(buttonCreateEdit);
		layoutEditActions.append(buttonCancel);

		layoutExerciseName.append(labelExerciseName);
		layoutExerciseName.append(editExerciseName);
		layoutExerciseName.append(checkPublic);
		layoutChecks1.append(checkHead,Size(SizeTypeMax,SizeTypeAuto));
		layoutChecks1.append(checkLeftHand, Size(SizeTypeMax, SizeTypeAuto));
		layoutChecks1.append(checkRightHand, Size(SizeTypeMax, SizeTypeAuto));
		layoutChecks2.append(checkHip, Size(SizeTypeMax, SizeTypeAuto));
		layoutChecks2.append(checkLeftFoot, Size(SizeTypeMax, SizeTypeAuto));
		layoutChecks2.append(checkRightFoot, Size(SizeTypeMax, SizeTypeAuto));

		layoutContentEditData.append(layoutExerciseName,AutoSize,20);
		layoutContentEditData.append(labelChecks);
		layoutContentEditData.append(layoutChecks1);
		layoutContentEditData.append(layoutChecks2, AutoSize, 20);
		layoutContentEditData.append(layoutEditActions);

		layout.append(layoutActions,AutoSize,20);
		layout.append(layoutContent);
		layout.setMargin(10);
	}

	Layout *ExercisesController::viewDefinition() {
		return &layout;
	}

	void ExercisesController::onViewEnter() {
		m_editMode = 0;
		m_currentSelection = 0;
		updateExerciseList();
	}

	void ExercisesController::onViewQuit() {

	}

}