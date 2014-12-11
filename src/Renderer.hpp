/*
 * Renderer.h
 *
 *  Created on: Jul 3, 2014
 *      Author: Christopher B. Choy
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include <vector>
#include <chrono>
#include <thread>

#include <osg/Group>
#include <osg/Hint>
#include <osg/ShadeModel>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osg/ComputeBoundsVisitor>
#include <osg/MatrixTransform>
#include <osgUtil/Optimizer>
#include <osgUtil/SmoothingVisitor>

#define PI 3.14159265359

namespace CLR {

// Interal class
class CaptureCB;

// Renderer main class
class Renderer {
protected:
	// Scene data
	osg::ref_ptr<osg::Group> sceneRoot;
    std::vector<std::string> modelNames;
    std::vector<osg::ref_ptr<osg::Node> > loadedModels;
	osg::ref_ptr<osgViewer::Viewer> viewer;

public:
	// Renderer setting
	bool offScreen;
	CaptureCB* callBack;

	// Camera related variables
	int screenWidth;
	int screenHeight;
	
    unsigned int current_model_index;

    double azimuth;
	double elevation;
	double yaw;
	double distance;
	double fieldOfView;

	// Object related variables
    std::vector<osg::BoundingSphere> boundingSpheres;
    std::vector<double> objectRadiuses;
    std::vector<osg::Vec3> objectCenters;

	Renderer();
	static void flipRendering(GLubyte *imageInput, int gWidth , int gHeight, GLubyte *imageOutput);
	static void flipDepth(float * depthInput, int gWidth, int gHeight, double * depthOutput);

//	void setViewport(int _screenWidth, int _screenHeight);
	void setViewpoint(double _azimuth,
			double _elevation,
			double _yaw,
			double _distance,
			double _fieldOfView);
    const osg::Matrixd& getViewMatrix();
    const osg::Matrixd& getProjectionMatrix();


//    void setViewpoint(double _azimuth,
//			double _elevation,
//			double _yaw,
//			double _distance,
//			double _fieldOfView,
//            int _model_index);
    std::vector<std::string> getModelNames();

// 0 base index
    void setModelIndex(int _modelIndex);
    bool addModel(std::string fileName);
	bool initialize(std::vector<std::string> fileNames,
			bool offScreen,
			int _screenWidth,
			int _screenHeight,
			double _azimuth,
			double _elevation,
			double _yaw,
			double _distance,
			double _fieldOfView);

	void render(unsigned char* rendering, double * depth);

	virtual ~Renderer();
};

/** Custom callback class that includes rendering
 *
 */
class CaptureCB: public osg::Camera::DrawCallback {
	osgViewer::Viewer* viewer;
	unsigned char * rendering;
	double * depth;
	bool offScreen;
	int width;
	int height;
public:
	CaptureCB(osgViewer::Viewer* _viewer, int _width, int _height, bool _offScreen) :
		viewer(_viewer),width(_width), height(_height), offScreen(_offScreen){
	}
	void setRendering(unsigned char * _rendering){rendering = _rendering;}
	void setDepth(double * _depth){depth = _depth;}
	virtual void operator()(osg::RenderInfo& ri) const{
		osg::ref_ptr<osg::Image> renderedImage = new osg::Image;
		renderedImage->readPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE);
		// osgDB::writeImageFile(*renderedImage, "rendered.png");
		Renderer::flipRendering(renderedImage->data(),width,height,rendering);

		if (depth != NULL){
			osg::ref_ptr<osg::Image> depthImage(new osg::Image);
			depthImage->readPixels(0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT);
			Renderer::flipDepth((float *) depthImage->data(), width, height, depth);
		}
		viewer->setDone(true);
	}

private:
	virtual ~CaptureCB() {
	}
};
    
//    class ShadeModelVisitor : public osg::NodeVisitor
//    {
//    public:
//        ShadeModelVisitor ()
//        : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN ){};
//        
//        // This method gets called for every node in the scene
//        //   graph. Check each node to see if its name matches
//        //   out target. If so, save the node's address.
//        virtual void apply( osg::Node& node ){
//            osg::StateSet* ss = node.getOrCreateStateSet();
//            osg::ShadeModel* sm = dynamic_cast<osg::ShadeModel*>(ss->getAttribute(osg::StateAttribute::SHADEMODEL ) );
//            if (sm != NULL){
//                sm->setMode( osg::ShadeModel::SMOOTH );
//                std::cout<<"model smooth"<<std::endl;
//            }
//            
//            // Keep traversing the rest of the scene graph.
//            traverse( node );
//        }
//        
//        osg::Node* getNode() { return _node.get(); }
//        
//    protected:
//        osg::ref_ptr<osg::Node> _node;
//    };


} /* namespace CLR */

#endif /* RENDERER_H_ */
