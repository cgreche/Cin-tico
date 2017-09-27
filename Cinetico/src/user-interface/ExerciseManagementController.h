
#ifndef __CINETICO_EXERCISEMANAGEMENTCONTROLLER_H__
#define __CINETICO_EXERCISEMANAGEMENTCONTROLLER_H__

#include "Controller.h"
#include "uilib/ui/uibase.h"

namespace cinetico {

	class ExerciseManagementController : public Controller
	{
	public:
		VerticalLayout layout;
			HorizontalLayout layoutAction;
				VerticalLayout layoutActionType;
					Label labelActionType;
					ComboBox comboActionType;
				VerticalLayout layoutMinTime;
					Label labelMinTime;
					EditBox editMinTime;
				VerticalLayout layoutMaxTime;
					Label labelMaxTime;
					EditBox editMaxTime;
		

			VerticalLayout layoutSpacing;
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

				HorizontalLayout layoutPosition;
					VerticalLayout layoutPositionX;
						Label labelPositionX;
						EditBox editPositionX;
					VerticalLayout layoutPositionY;
						Label labelPositionY;
						EditBox editPositionY;
					VerticalLayout layoutPositionZ;
						Label labelSpaceZ;
						EditBox editPositionZ;

			VerticalLayout layoutSpecific;
			//Position action
				HorizontalLayout layoutMinHoldTime;
					Label labelMinHoldtimeX;
					EditBox editMinHoldtimeX;

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

	public:
		ExerciseManagementController();

		Layout *viewDefinition();
		void onViewEnter();
		void onViewQuit();
	};

}

#endif