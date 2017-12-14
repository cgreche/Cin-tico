#ifndef __CINETICO_GENERAL_SETTINGS_H__
#define __CINETICO_GENERAL_SETTINGS_H__

#include <string>

namespace cinetico {

	class GeneralSettings
	{
		float m_posDistThreshold;
		float m_posMinHoldtime;
		int m_adapter;
		int m_resolutionWidth;
		int m_resolutionHeight;
		int m_refreshRate;
		bool m_fullscreen;
		bool m_antialiasing;
		
	public:
		GeneralSettings();
		
		void setPosDistThreshold(float posDistThreshold) { m_posDistThreshold = posDistThreshold; }
		void setPosMinHoldtime(float minHoldtime) { m_posMinHoldtime = minHoldtime; }
		void setAdapter(int adapter) { m_adapter = adapter; }
		void setResolutionWidth(int width) { m_resolutionWidth = width; }
		void setResolutionHeight(int height) { m_resolutionHeight = height; }
		void setRefreshRate(int refreshRate) { m_refreshRate = refreshRate; }
		void setFullscreen(bool fullscreen) { m_fullscreen = fullscreen; }
		void setAntialiasing(bool antialiasing) { m_antialiasing = antialiasing; }
		
		float posDistThreshold() const { return m_posDistThreshold; }
		float posMinHoldtime() const { return m_posMinHoldtime; }
		int adapter() const { return m_adapter; }
		int resolutionWidth() const { return m_resolutionWidth; }
		int resolutionHeight() const { return m_resolutionHeight; }
		int refreshRate() const { return m_refreshRate; }
		bool fullscreen() const { return m_fullscreen; }
		bool antialiasing() const { return m_antialiasing; }
	};

}

#endif