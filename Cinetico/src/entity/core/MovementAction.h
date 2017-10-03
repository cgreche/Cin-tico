#ifndef __CINETICO_CORE_MOVEMENTACTION_H__
#define __CINETICO_CORE_MOVEMENTACTION_H__

#include "action.h"

namespace cinetico_core {

	class MovementAction : public Action {
	public:
		MovementAction()
			: Action(Action::Movement) {
		}
	};

}

#endif