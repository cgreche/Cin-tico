#ifndef __CINETICO_CORE_ACTION_H__
#define __CINETICO_CORE_ACTION_H__

namespace cinetico_core {
	class ActionResult {
		Missed,
		Bad,
		Good,
		Excelent
	};

	class Action {
		//Definition attributes
		std::string m_tag;
		float minTime;
		float maxTime;

		//State attributes
		bool m_correct;
	public:
		Action();
		bool isCorrect() const { return m_correct; }
	};

}

#endif