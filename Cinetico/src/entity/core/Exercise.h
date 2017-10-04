#ifndef __CINETICO_CORE_EXERCISE_H__
#define __CINETICO_CORE_EXERCISE_H__

#include <vector>

#include "action.h"

namespace cinetico_core {

	class Exercise {
		unsigned long m_id;
		std::string m_name;
		std::string m_author;
		bool m_public;
		unsigned long m_trackableBodyPoints;

		std::vector<Action*> m_actions;

	public:
		Exercise(unsigned long id = -1);
		~Exercise();

		void insertAction(Action *action);

		void setName(const char *name) { m_name = name; }
		void setAuthor(const char *author) { m_author = author; }
		void setPublic(bool isPublic) { m_public = isPublic; }
		void setTrackableBodyPoints(unsigned long trackableBodyPoints) { m_trackableBodyPoints = trackableBodyPoints; }
		void setActionList(std::vector<Action*> actionList) { m_actions = actionList; }

		unsigned long id() const { return m_id; }
		const std::string& name() const { return m_name; }
		const std::string& author() const { return m_author; }
		bool isPublic() const { return m_public; }
		unsigned long trackableBodyPoints() const { return m_trackableBodyPoints; }
		Action *action(int index) const { return m_actions[index]; }
		unsigned int actionCount() const { return m_actions.size(); }
	};

}

#endif