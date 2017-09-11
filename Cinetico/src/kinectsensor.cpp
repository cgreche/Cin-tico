
#define SAFE_RELEASE(x) if(x) { x->Release(); x = NULL; }
#pragma comment (lib, "Kinect20.lib")

#include "kinectsensor.h"

namespace cinetico_core {

	KinectSensor::KinectSensor() {

	}

	bool KinectSensor::initialize() {
		HRESULT hr;

		hr = ::GetDefaultKinectSensor(&m_pKinectSensor);
		if (FAILED(hr))
		{
			return hr;
		}

		if (m_pKinectSensor)
		{
			// Initialize the Kinect and get coordinate mapper and the body reader
			IBodyFrameSource* pBodyFrameSource = NULL;

			hr = m_pKinectSensor->Open();

			if (SUCCEEDED(hr))
			{
				hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
			}

			if (SUCCEEDED(hr))
			{
				hr = m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);
			}

			if (SUCCEEDED(hr))
			{
				hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);
			}

			SAFE_RELEASE(pBodyFrameSource);
		}

		return !m_pKinectSensor || FAILED(hr);
	}

	void KinectSensor::update() {
		if (!m_pBodyFrameReader)
			return;

		IBodyFrame* pBodyFrame = NULL;

		HRESULT hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);
		if (SUCCEEDED(hr)) {
			INT64 nTime = 0;

			hr = pBodyFrame->get_RelativeTime(&nTime);
			IBody* ppBodies[BODY_COUNT] = { 0 };

			if (SUCCEEDED(hr))
				hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);

			for (int i = 0; i < _countof(ppBodies); ++i)
			{
				SAFE_RELEASE(ppBodies[i]);
			}
		}

		SAFE_RELEASE(pBodyFrame);
	}

	void KinectSensor::finalize() {

	}

	long KinectSensor::checkCapabilities(long capabilities) {
		return 0;
	}

	u8 *KinectSensor::getDepthImageData() {
		return NULL;
	}

}