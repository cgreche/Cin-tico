#ifndef __CINETICO_CORE_ACTION_H__
#define __CINETICO_CORE_ACTION_H__

#include <string>

namespace cinetico_core {

	enum ActionResult {
		Missed,
		Bad,
		Good,
		Excelent
	};

	class Action {
	public:
		enum ActionType {
			Position,
			Movement
		};

	protected:
		int m_id;
		ActionType m_actionType;

		//Definition attributes
		std::string m_tag;
		float minTime;
		float maxTime;

		//State attributes
		bool m_correct;
	protected:
		Action(ActionType actionType, int id = -1)
			: m_actionType(actionType), m_id(id) {
		}

	public:
		int id() const { return m_id; }
		ActionType type() const { return m_actionType; }
		const std::string& tag() const { return m_tag; }
		bool isCorrect() const { return m_correct; }
	};

}

#endif