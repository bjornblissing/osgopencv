/*
 * opencvimagestream.cpp
 *
 *  Created on: Oct 18, 2014
 *      Author: Bjorn Blissing
 */
#include "opencvimagestream.h"
#include <iostream>

OpenCVImageStream::OpenCVImageStream() : m_cameraThread(0) {}

OpenCVImageStream::~OpenCVImageStream() {
	if (m_cameraThread->isRunning()) {
		m_cameraThread->cancel();
	}
	
	delete m_cameraThread;
}

bool OpenCVImageStream::openCamera(int deviceId)
{
	m_cameraThread = new OpenCVCameraThread(deviceId);
	m_cameraThread->start();
	return true;
}

void OpenCVImageStream::update(osg::NodeVisitor* /*nv*/)
{
	if (m_cameraThread->initialized()) {
		m_cameraThread->getData(m_frame);
		this->setImage(m_cameraThread->sensorSizeX(), m_cameraThread->sensorSizeY(), 1, m_cameraThread->internalTextureFormat(), m_cameraThread->pixelFormat(), m_cameraThread->dataType(), (unsigned char*)(m_frame.data), osg::Image::NO_DELETE, 1);
	}
}

OpenCVCameraThread::OpenCVCameraThread(int deviceId) : OpenThreads::Thread(), 
	m_done(false),
	m_videoCaptureDevice(0), 
	m_init(false),
	m_sensorSizeX(0),
	m_sensorSizeY(0),
	m_frameRate(0.0),
	m_dataType(GL_UNSIGNED_BYTE),
	m_pixelFormat(GL_BGR),
	m_internalTextureFormat(GL_RGB) 
{
	// Open video device
	m_videoCaptureDevice = new cv::VideoCapture(deviceId);

	// check if we succeeded
	if (!m_videoCaptureDevice->isOpened()) {
		return;
	}

	// Get camera properties
	m_sensorSizeX = static_cast<int> (m_videoCaptureDevice->get(cv::CAP_PROP_FRAME_WIDTH));
	m_sensorSizeY = static_cast<int> (m_videoCaptureDevice->get(cv::CAP_PROP_FRAME_HEIGHT));
	m_frameRate = m_videoCaptureDevice->get(cv::CAP_PROP_FPS);

	// Grab one image to determine format
	m_videoCaptureDevice->grab();
	m_videoCaptureDevice->retrieve(m_frame);
	// Make sure that grabbed images is continous
	if (!m_frame.isContinuous()) {
		return;
	}

	// Get the data type of image
	switch (m_frame.depth()) {
		case CV_8U:
			m_dataType = GL_UNSIGNED_BYTE;
			break;
		case CV_8S:
			m_dataType = GL_BYTE;
			break;
		default:
			std::cerr << "Error: Unknown data type." << std::endl;
			return;
			break;
	}

	// Get the pixelformat of image
	switch (m_frame.channels()) {
		case 3:
			m_pixelFormat = GL_BGR;
			m_internalTextureFormat = GL_RGB;
			break;
		case 1:
			m_pixelFormat = GL_LUMINANCE;
			m_internalTextureFormat = GL_LUMINANCE;
			break;
		default:
			std::cerr << "Error: Unknown pixel format." << std::endl;
			return;
			break;
	}

	m_init = true;
}

void OpenCVCameraThread::run()
{
	while (!m_done) {
		if (m_init) {
			// Get new image
			m_videoCaptureDevice->grab();
			m_videoCaptureDevice->retrieve(m_frame);
		}

		OpenThreads::Thread::microSleep(1);
	}
}

int OpenCVCameraThread::cancel()
{
	m_done = true;

	// Release camera device
	if (m_videoCaptureDevice->isOpened()) {
		m_videoCaptureDevice->release();
		
	}
	m_init = false;

	delete m_videoCaptureDevice;

	return 0;
}

void OpenCVCameraThread::getData(cv::Mat& frame) {
	// Copy image in thread to frame
	m_frame.copyTo(frame);
}

