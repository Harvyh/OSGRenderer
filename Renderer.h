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
#include <mexplus.h>
#include <mex.h>

#define PI 3.14159265359
using namespace mexplus;

namespace CLR {

class CaptureCB;
class Renderer {
protected:
	// Scene data
	osg::ref_ptr<osg::Group> sceneRoot;
    std::vector<std::string> modelNames;
    std::vector<osg::ref_ptr<osg::Node>> loadedModels;
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

//    void setViewpoint(double _azimuth,
//			double _elevation,
//			double _yaw,
//			double _distance,
//			double _fieldOfView,
//            int _model_index);
    std::vector<std::string> getModelNames();

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

#define MATLAB
#ifdef MATLAB

// This initializes the instance session storage for Database.
template class mexplus::Session<CLR::Renderer>;


namespace {

// Create a new instance of Renderer and return its session id.
MEX_DEFINE(new) (int nlhs, mxArray* plhs[],
                 int nrhs, const mxArray* prhs[]) {
    InputArguments input(nrhs, prhs, 0);
    OutputArguments output(nlhs, plhs, 1);
    output.set(0, Session<CLR::Renderer>::create(new CLR::Renderer()));
}

// Delete the Renderer instance specified by its id.
MEX_DEFINE(delete) (int nlhs, mxArray* plhs[],
                    int nrhs, const mxArray* prhs[]) {
    InputArguments input(nrhs, prhs, 1);
    OutputArguments output(nlhs, plhs, 0);
    Session<CLR::Renderer>::destroy(input.get(0));
}

// Initialize the renderer
MEX_DEFINE(initialize) (int nlhs, mxArray* plhs[],
                    int nrhs, const mxArray* prhs[]) {
    InputArguments input(nrhs, prhs, 10); // 1 for id and 9 for input arguments
    OutputArguments output(nlhs, plhs, 1); // 1 for boolean success flag
    CLR::Renderer* renderer = Session<CLR::Renderer>::get(input.get(0));
    std::vector<std::string> file_names = MxArray::to<std::vector<std::string> >(prhs[1]);
 
    // DEBUG
    // std::cout<<value[0]<<std::endl;
    // END DEBUG
    
    bool success = renderer->initialize(file_names,//input.get<std::vector<std::string>>(1), // file name
      	  input.get<bool>(2), // off screen flag
      	  input.get<int>(3), // screen width
      	  input.get<int>(4), // screen height
      	  input.get<double>(5), // azimuth
      	  input.get<double>(6), // elevation
      	  input.get<double>(7), // yaw
      	  input.get<double>(8), // distance
      	  input.get<double>(9) // field of view
      	);
    output.set(0, success);
}

MEX_DEFINE(render) (int nlhs, mxArray* plhs[],
                   int nrhs, const mxArray* prhs[]) {
	bool renderDepth = !(nlhs == 1);
	InputArguments input(nrhs, prhs, 1);
	OutputArguments output(nlhs, plhs, (renderDepth)?2:1);

	CLR::Renderer* renderer = Session<CLR::Renderer>::get(input.get(0));
	const mwSize dims[] = { renderer->screenHeight, renderer->screenWidth, 3 };
	plhs[0] = mxCreateNumericArray(3, dims, mxUINT8_CLASS, mxREAL);
	unsigned char * imageOutput = (unsigned char *) mxGetPr(plhs[0]);

	double *depthOutput = NULL;

	if (renderDepth){
		plhs[1] = mxCreateDoubleMatrix(renderer->screenHeight, renderer->screenWidth, mxREAL);
		depthOutput = mxGetPr(plhs[1]);
	}
	renderer->render(imageOutput, depthOutput);
	output.set(0,plhs[0]);
	if (renderDepth) output.set(1,plhs[1]);
}

MEX_DEFINE(addModel) (int nlhs, mxArray* plhs[],
                   int nrhs, const mxArray* prhs[]) {
    InputArguments input(nrhs, prhs, 2);
    OutputArguments output(nlhs, plhs, 1);
    
    CLR::Renderer* renderer = Session<CLR::Renderer>::get(input.get(0));
    bool success = renderer->addModel(input.get<std::string>(1));
	output.set(0,success);
}

MEX_DEFINE(setModelIndex) (int nlhs, mxArray* plhs[],
                   int nrhs, const mxArray* prhs[]) {
    InputArguments input(nrhs, prhs, 2);
    OutputArguments output(nlhs, plhs, 0);

    CLR::Renderer* renderer = Session<CLR::Renderer>::get(input.get(0));
	renderer->setModelIndex(input.get<int>(1));
}

MEX_DEFINE(setViewpoint) (int nlhs, mxArray* plhs[],
                   int nrhs, const mxArray* prhs[]) {
	InputArguments input(nrhs, prhs, 6);
	OutputArguments output(nlhs, plhs, 0);

	CLR::Renderer* renderer = Session<CLR::Renderer>::get(input.get(0));
	renderer->setViewpoint(input.get<double>(1), input.get<double>(2), input.get<double>(3), input.get<double>(4), input.get<double>(5));
}

//MEX_DEFINE(setViewport) (int nlhs, mxArray* plhs[],
//                   int nrhs, const mxArray* prhs[]) {
//	InputArguments input(nrhs, prhs, 3);
//	OutputArguments output(nlhs, plhs, 0);
//
//	CLR::Renderer* renderer = Session<CLR::Renderer>::get(input.get(0));
//	renderer->setViewport(input.get<int>(1), input.get<int>(2));
//}

} // End of Namespace


MEX_DISPATCH

#endif /* MATLAB */

#endif /* RENDERER_H_ */
