/*
 * opencvimagestream.cpp
 *
 *  Created on: Oct 18, 2014
 *      Author: Bjorn Blissing
 */
#include "opencvimagestream.h"
#include <iostream>

OpenCVImageStream::OpenCVImageStream() : m_videoCaptureDevice(0), m_init(false),
	m_sensorSizeX(0),
	m_sensorSizeY(0),
	m_frameRate(0.0),
	m_dataType(GL_UNSIGNED_BYTE),
	m_pixelFormat(GL_BGR),
	m_internalTextureFormat(GL_RGB)
{
}

OpenCVImageStream::~OpenCVImageStream()
{
	if (m_videoCaptureDevice->isOpened()) {
		m_videoCaptureDevice->release();
	}
	delete m_videoCaptureDevice;
}

bool OpenCVImageStream::openCamera(int deviceId)
{
	// Open video device
	m_videoCaptureDevice = new cv::VideoCapture(deviceId);
	
	// check if we succeeded
	if (!m_videoCaptureDevice->isOpened()) {  
		return false;
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
		return false;
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
			return false;
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
			return false;
			break;
	}
	

	m_init = true;
	return true;
}

void OpenCVImageStream::update(osg::NodeVisitor* /*nv*/)
{
	if (m_init) {
		m_videoCaptureDevice->grab();
		m_videoCaptureDevice->retrieve(m_frame);
		this->setImage(m_sensorSizeX, m_sensorSizeY, 1, m_internalTextureFormat, m_pixelFormat, m_dataType, (unsigned char*)(m_frame.data), osg::Image::NO_DELETE, 1);
	}
}