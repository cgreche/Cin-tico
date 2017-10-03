#ifndef __CINETICO_CORE_MOVEMENTACTION_H__
#define __CINETICO_CORE_MOVEMENTACTION_H__

#include "action.h"

namespace cinetico_core {

	class MovementAction : public Action {
	public:
		enum MovementType {
			Linear,
			Angular
		};

		MovementAction()
			: Action(Action::Movement) {
		}
	};

}

#endif