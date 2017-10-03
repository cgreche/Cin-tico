
#ifndef __CINETICO_EXERCISEMANAGEMENTCONTROLLER_H__
#define __CINETICO_EXERCISEMANAGEMENTCONTROLLER_H__

#include "Controller.h"
#include "uilib/ui/uibase.h"

namespace cinetico {

	using namespace uilib;

	class ExerciseManagementController : public Controller
	{
	public:
		VerticalLayout layout;
			VerticalLayout layoutActions;
				Label labelViewTitle;
				Label labelViewDescr;
				HorizontalLayout layoutActionButtons;
					Button buttonSaveExercise;
					Button buttonCancelExercise;

			VerticalLayout layoutContent;

			HorizontalLayout layoutContentList;
				GridView gridActions;
				VerticalLayout layoutGridActions;
					Button buttonAdd;
					Button buttonEdit;
					Button buttonDelete;

			VerticalLayout layoutActionData;
				HorizontalLayout layoutActionDataActionButtons;
					Button buttonSaveAction;
					Button buttonCancelAction;

				HorizontalLayout layoutActionDataRow1;
					VerticalLayout layoutName;
						Label labelName;
						EditBox editName;
					VerticalLayout layoutPartOf;
						Label labelPartOf;
						ComboBox comboPartOf;
				HorizontalLayout layoutBaseActionData;
					VerticalLayout layoutActionType;
						Label labelActionType;
						ComboBox comboActionType;
					VerticalLayout layoutMinTime;
						Label labelMinTime;
						EditBox editMinTime;
					VerticalLayout layoutMaxTime;
						Label labelMaxTime;
						EditBox editMaxTime;
		
				Label labelSpaceType;
				HorizontalLayout layoutSpaceType;
					VerticalLayout layoutSpaceTypeX;
						Label labelSpaceTypeX;
						ComboBox comboSpaceTypeX;
					VerticalLayout layoutSpaceTypeY;
						Label labelSpaceTypeY;
						ComboBox comboSpaceTypeY;
					VerticalLayout layoutSpaceTypeZ;
						Label labelSpaceTypeZ;
						ComboBox comboSpaceTypeZ;

				Label labelPosition;
				HorizontalLayout layoutPosition;
					VerticalLayout layoutPositionX;
						Label labelPositionX;
						EditBox editPositionX;
					VerticalLayout layoutPositionY;
						Label labelPositionY;
						EditBox editPositionY;
					VerticalLayout layoutPositionZ;
						Label labelPositionZ;
						EditBox editPositionZ;

				VerticalLayout layoutSpecific;
				//Position action
					VerticalLayout layoutMinHoldTime;
						Label labelMinHoldtime;
						EditBox editMinHoldtime;

				//Movement action
					HorizontalLayout layoutMovementAction;
						VerticalLayout layoutMovementType;
							Label labelMovementType;
							ComboBox comboMovementType;
						VerticalLayout layoutMinSpeed;
							Label labelMinSpeed;
							EditBox editMinSpeed;
						VerticalLayout layoutMaxSpeed;
							Label labelMaxSpeed;
							EditBox editMaxSpeed;

		Exercise *m_currentExercise;
		int m_editMode;
	public:
		ExerciseManagementController();

		Layout *viewDefinition();
		void onViewEnter(ViewParams *params);
		void onViewQuit();

		void setEditionMode(int mode);
	};

}

#endif