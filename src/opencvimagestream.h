/*
 * opencvimagestream.h
 *
 *  Created on: Oct 18, 2014
 *      Author: Bjorn Blissing
 */

#ifndef _OPENCV_IMAGESTREAM_H_
#define _OPENCV_IMAGESTREAM_H_

#include <osg/ImageStream>
#include <OpenThreads/Thread>
#include <OpenThreads/Mutex>

#include <opencv2/opencv.hpp>

class OpenCVCameraThread : public OpenThreads::Thread {
public:
	OpenCVCameraThread(int deviceId);
	virtual void run();
	virtual int cancel();
	bool initialized() const { return m_init; }
	int sensorSizeX() const { return m_sensorSizeX; }
	int sensorSizeY() const { return m_sensorSizeY; }
	double getFrameRate() const { return m_frameRate; }
	GLenum dataType() const { return m_dataType; }
	GLenum pixelFormat() const { return m_pixelFormat; }
	GLenum internalTextureFormat() const { return m_internalTextureFormat; }
	void getData(cv::Mat& frame);
	bool newImageAvailable() const { return m_newImageAvailable; }
protected:
	bool m_init;
	bool m_done;
	bool m_newImageAvailable;
	cv::VideoCapture* m_videoCaptureDevice;
	cv::Mat m_backBuffer;
	cv::Mat m_frontBuffer;
	int m_sensorSizeX;
	int m_sensorSizeY;
	double m_frameRate;
	OpenThreads::Mutex m_mutex;
	GLenum m_dataType;
	GLenum m_pixelFormat;
	GLenum m_internalTextureFormat;
};

class OpenCVImageStream : public osg::ImageStream {
	public:
		OpenCVImageStream();

		bool openCamera(int deviceId);
		int sensorSizeX() const { return m_cameraThread ? m_cameraThread->sensorSizeX() : 0; }
		int sensorSizeY() const { return m_cameraThread ? m_cameraThread->sensorSizeY() : 0; }
		double getFrameRate() const { return m_cameraThread ? m_cameraThread->getFrameRate() : 0.0; }
		float aspectRatio() const { return m_cameraThread->sensorSizeY() != 0 ? float(sensorSizeX()) / float(sensorSizeY()) : 1.0f; }

		/** ImageSequence requires a call to update(NodeVisitor*) during the update traversal so return true.*/
		virtual bool requiresUpdateCall() const { return true; }
		virtual void update(osg::NodeVisitor* nv);

	protected:
		~OpenCVImageStream(); // Since we inherit from osg::Referenced we must make destructor protected
		OpenCVCameraThread* m_cameraThread;
		cv::Mat m_frame;
};

#endif
