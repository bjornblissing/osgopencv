/*
 * opencvimagestream.h
 *
 *  Created on: Oct 18, 2014
 *      Author: Bjorn Blissing
 */

#ifndef _OPENCV_IMAGESTREAM_H_
#define _OPENCV_IMAGESTREAM_H_

#include <osg/ImageStream>
#include <opencv2/opencv.hpp>

class OpenCVImageStream : public osg::ImageStream {
	public:
		OpenCVImageStream();

		bool openCamera(int deviceId);
		double getFrameRate() const { return m_frameRate; }
		int sensorSizeX() const { return m_sensorSizeX; }
		int sensorSizeY() const { return m_sensorSizeY; }
		float aspectRatio() const { return float(m_sensorSizeX)/float(m_sensorSizeY); }

		/** ImageSequence requires a call to update(NodeVisitor*) during the update traversal so return true.*/
		virtual bool requiresUpdateCall() const { return true; }
		virtual void update(osg::NodeVisitor* nv);

	private:
		~OpenCVImageStream(); // Since we inherit from osg::Referenced we must make destructor protected

		cv::VideoCapture* m_videoCaptureDevice;
		cv::Mat m_frame;
		bool m_init;
		int m_sensorSizeX;
		int m_sensorSizeY;
		double m_frameRate;
		GLenum m_dataType;
		GLenum m_pixelFormat;
		GLenum m_internalTextureFormat;
};

#endif
