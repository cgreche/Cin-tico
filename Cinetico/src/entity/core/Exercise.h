#ifndef __CINETICO_CORE_EXERCISE_H__
#define __CINETICO_CORE_EXERCISE_H__

namespace cinetico_core {

	class Exercise {
		unsigned long m_id;
		std::string m_name;
		std::string m_author;
		bool m_public;

	public:
		Exercise();
	};

}

#endif