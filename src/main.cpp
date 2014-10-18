/*
 * main.cpp
 *
 *  Created on: Oct 18, 2014
 *      Author: Bjorn Blissing
 */

#include <iostream>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/PolygonMode>
#include <osg/Texture2D>
#include <osg/Shader>
#include <osgGA/StateSetManipulator>

#include "opencvimagestream.h"


osg::Geode* createHUDQuad( float width, float height)
{
	osg::ref_ptr<osg::Geometry> geom = osg::createTexturedQuadGeometry(osg::Vec3(0.0f,0.0f,0.0f),
									   osg::Vec3(width,0.0f,0.0f),
									   osg::Vec3(0.0f,0.0f,height),
									   0.0f,
									   1.0f,
									   1.0f,
									   0.0f);
	osg::ref_ptr<osg::Geode> quad = new osg::Geode;
	quad->addDrawable( geom );
	int values = osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED;
	quad->getOrCreateStateSet()->setAttribute(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL), values );
	quad->getOrCreateStateSet()->setMode( GL_LIGHTING, values );
	return quad.release();
}

osg::Geode* createCameraPlane(unsigned long cameraId) {
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	texture->setTextureSize( 1280, 1024);
	texture->setFilter(osg::Texture::MIN_FILTER , osg::Texture::LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER , osg::Texture::LINEAR);
	texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
	texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
	texture->setResizeNonPowerOfTwoHint(false);
	// Create OpenCVImageStream
	osg::ref_ptr<OpenCVImageStream> openCVImageStream = new OpenCVImageStream();
	openCVImageStream->openCamera(cameraId);
	texture->setImage(openCVImageStream);
	osg::ref_ptr<osg::Geode> quadGeode = createHUDQuad(openCVImageStream->aspectRatio(), 1.0f);

	// Apply texture to quad
	osg::ref_ptr<osg::StateSet> stateSet = quadGeode->getOrCreateStateSet();
	stateSet->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);

	return quadGeode.release();	
}

int main( int argc, char** argv )
{
	// use an ArgumentParser object to manage the program arguments.
	osg::ArgumentParser arguments(&argc,argv);
	// Create viewer
	osgViewer::Viewer viewer(arguments);
	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(createCameraPlane(0));
	
	viewer.addEventHandler(new osgViewer::StatsHandler());
	viewer.setSceneData(root);
	// Start Viewer
	viewer.run();
	return 0;
}
