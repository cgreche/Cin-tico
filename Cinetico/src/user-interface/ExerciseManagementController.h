
#ifndef __CINETICO_EXERCISEMANAGEMENTCONTROLLER_H__
#define __CINETICO_EXERCISEMANAGEMENTCONTROLLER_H__

#include "Controller.h"
#include "uilib/ui/uibase.h"

namespace cinetico {

	class ExerciseManagementController : public Controller
	{
	public:
		VerticalLayout layout;

	public:
		ExerciseManagementController();

		Layout *viewDefinition();
		void onViewEnter();
		void onViewQuit();
	};

}

#endif