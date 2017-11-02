
#define SAFE_RELEASE(x) if(x) { x->Release(); x = NULL; }
#pragma comment (lib, "Kinect20.lib")

#include "kinectsensor.h"

namespace cinetico_core {

	KinectSensor::KinectSensor() {
		m_pKinectSensor = NULL;
		m_pCoordinateMapper = NULL;
		m_pBodyFrameReader = NULL;
		m_identifiedBodyCount = 0;
	}

	KinectSensor::~KinectSensor() {
		finalize();
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
				hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);

			if (SUCCEEDED(hr))
				hr = m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);

			if (SUCCEEDED(hr))
				hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);

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
			m_identifiedBodyCount = 0;

			if (SUCCEEDED(hr))
				hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);

			for (int i = 0; i < _countof(ppBodies); ++i) {
				BOOLEAN bTracked = false;
				hr = ppBodies[i]->get_IsTracked(&bTracked);
				m_identifiedBodies[i] = SUCCEEDED(hr) && bTracked;
				if(m_identifiedBodies[i]) {
					m_identifiedBodyCount++;
					ppBodies[i]->GetJointOrientations(JointType_Count, m_orientations[i]);
					ppBodies[i]->GetJoints(JointType_Count, m_joints[i]);
				}
			}

			for (int i = 0; i < _countof(ppBodies); ++i)
				SAFE_RELEASE(ppBodies[i]);
		}

		SAFE_RELEASE(pBodyFrame);
	}

	void KinectSensor::finalize() {
		if(m_pBodyFrameReader)
			m_pBodyFrameReader->Release();

		if(m_pCoordinateMapper)
			m_pCoordinateMapper->Release();

		if(m_pKinectSensor)
			m_pKinectSensor->Release();
	}

	unsigned long KinectSensor::getCapabilities() {
		return Sensor::DEPTH_IMAGE | Sensor::SKELETON_TRACKING;
	}

	u8 *KinectSensor::getDepthImageData() {
		return NULL;
	}

}