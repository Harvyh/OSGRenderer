/*
 * Renderer.cpp
 *
 *  Created on: Jul 3, 2014
 *      Author: Christopher B. Choy
 */

#include "Renderer.hpp"

namespace CLR {


Renderer::Renderer() {
	viewer = new osgViewer::Viewer();
}

Renderer::~Renderer() {
	// TODO Auto-generated destructor stub
}

/* ------------------------------------------------------------------------
 * Modified by Wonhui Kim kwh6465@gmail.com
 * createLightSource - creates light sources from the scene graph
 *-----------------------------------------------------------------------*/
osg::LightSource* Renderer::createLightSource(
        unsigned int num,
        const osg::Vec3& trans,
        const osg::Vec4& color)
{
    // create a new light
    osg::ref_ptr<osg::Light> light = new osg::Light;
    light->setLightNum(num);
    light->setDiffuse(color);
    light->setPosition( osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f) );

    // assign the light to a newly-created osg::LightSource node
    osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource;
    lightSource->setLight( light );

    // add the light source to a translated osg::MatrixTransform node
    osg::ref_ptr<osg::MatrixTransform> sourceTrans = new osg::MatrixTransform;
    sourceTrans->setMatrix( osg::Matrix::translate(trans) );
    sourceTrans->addChild( lightSource.get() );
    sourceTrans.release();

    return lightSource;
}
/* ----------------------------------------------------------------------
 * Edit End
 *----------------------------------------------------------------------*/

// OSG fails
//void Renderer::setViewport(int _screenWidth, int _screenHeight){
//	screenWidth = _screenWidth;
//	screenHeight = _screenHeight;
//	std::cout<<"Viewport set"<<std::endl;
//	viewer->getCamera()->setViewport(0, 0, screenWidth, screenHeight);
//}
void Renderer::setModelIndex(int _model_index){
    if ((_model_index < 0) || (_model_index > loadedModels.size())){
        std::cout<<"Index out of bound"<<std::endl;
        return;
    }

    current_model_index = _model_index;
    for(int model_index = 0; model_index < loadedModels.size(); model_index++){
        loadedModels[model_index]->setNodeMask(model_index == current_model_index ? 0xffffffff : 0x0);
        //std::cout<<"Model "<< model_index << " set :"<<(model_index == current_model_index ? "visible" : "invisible") <<std::endl;
    }
}

// void Renderer::setViewpoint(double _azimuth, double _elevation, double _yaw, double _distance, double _fieldOfView){
//    setViewpoint(_azimuth, _elevation, _yaw, _distance, _fieldOfView, current_model_index);
//}

void Renderer::setViewpoint(double _azimuth, double _elevation, double _yaw, double _distance_ratio, double _fieldOfView){
	azimuth = _azimuth;
	elevation = _elevation;
	yaw = _yaw;
	fieldOfView = _fieldOfView;

	osg::Matrix rotx = osg::Matrix::rotate(elevation* PI/180.0f,osg::Vec3(1., 0., 0.));
	osg::Matrix roty = osg::Matrix::rotate(yaw* PI/180.0f, 		osg::Vec3(0., 1., 0.));
	osg::Matrix rotz = osg::Matrix::rotate(azimuth* PI/180.0f, 	osg::Vec3(0., 0., 1.));

	osg::Vec3 offset = rotz * rotx * osg::Vec3(0., -1., 0.);

    if (_distance_ratio <= 0 ){
    	distance = objectRadiuses[current_model_index] / tan(fieldOfView * PI/360.0f); // fieldOfView * PI/180 / 2
    } else {
    	distance = _distance_ratio * objectRadiuses[current_model_index] / tan(fieldOfView * PI/360.0f); // fieldOfView * PI/180 / 2
    }

	osg::Vec3 eye = objectCenters[current_model_index] + osg::Vec3( distance * offset.x(), distance * offset.y(), distance * offset.z()) ;
	osg::Vec3 up = rotz * roty * osg::Vec3(0.0f, 0.0f, 1.0f);
	osg::Matrix m = osg::Matrix::lookAt(eye, objectCenters[current_model_index], up);

	viewer->getCamera()->setViewMatrix( m );
	viewer->getCamera()->setProjectionMatrixAsPerspective( (fieldOfView <= 0)?25:fieldOfView, 1, 1 ,100000);
}

//void Renderer::setViewmatrix(double _viewMatrix){
//	osg::Matrix m = osg::Matrix::lookAt(eye, objectCenters[current_model_index], up);
//
//	viewer->getCamera()->setViewMatrix( m );
//	viewer->getCamera()->setProjectionMatrixAsPerspective( (fieldOfView <= 0)?25:fieldOfView, 1, 1 ,100000);
//}

const osg::Matrixd& Renderer::getViewMatrix(){
	return viewer->getCamera()->getViewMatrix();
}

const osg::Matrixd& Renderer::getProjectionMatrix(){
	return viewer->getCamera()->getProjectionMatrix();
}

bool Renderer::initialize(strvec fileNames,
  		bool _offScreen,
  		int _screenWidth,
  		int _screenHeight,
  		double _azimuth,
  		double _elevation,
  		double _yaw,
  		double _distance,
  		double _fieldOfView){
  	screenWidth = _screenWidth;
  	screenHeight = _screenHeight;
  	offScreen = _offScreen;

    // osgUtil::Optimizer optimizer;
    osgUtil::Optimizer::TextureVisitor tv(true, false, false, false, false, false);
    osgUtil::SmoothingVisitor sv;

    for (int fileIndex = 0; fileIndex < fileNames.size(); fileIndex++){
        // Return NULL on failure
        loadedModels.push_back(osgDB::readNodeFile(fileNames[fileIndex]));

        // Fail to load object
    	if (!loadedModels[fileIndex]){
    	    std::cout<<"Cannot load the model : " << fileNames[fileIndex] << std::endl;
            loadedModels.clear();
    	    return false;
    	}

        // Optimizer
        // optimizer.optimize(loadedModels[fileIndex].get());
        // Texture
        loadedModels[fileIndex]->accept(tv);
        // Smooth faces
        loadedModels[fileIndex]->accept(sv);
        std::cout<<"Model Loaded:"<<fileNames[fileIndex]<<std::endl;
    }

    // STD deep copy
    modelNames = fileNames;

//    ShadeModelVisitor smv;
//    loadedModel->accept(smv);

  	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
  	traits->x = 0;
  	traits->y = 0;
  	traits->width = screenWidth;
  	traits->height = screenHeight;
  	// traits->samples = 8; // Anti aliasing
  	if (offScreen) {
  		traits->windowDecoration = false;
  		traits->doubleBuffer = true;
  		traits->pbuffer = true;
  	} else {
  		traits->windowDecoration = true;
  		traits->doubleBuffer = true;
  		traits->pbuffer = false;
  	}
  	traits->sharedContext = 0;
  	// traits->alpha = 8;
  	std::cout << "DisplayName : " << traits->displayName() << std::endl;
  	traits->readDISPLAY();

  	osg::GraphicsContext* _gc = osg::GraphicsContext::createGraphicsContext(traits.get());
  	// osg::DisplaySettings::instance()->setNumMultiSamples( 16 ); // Anti Aliasing

  	if (!_gc) {
  		osg::notify(osg::NOTICE)<< "Failed to create pbuffer, failing back to normal graphics window." << std::endl;
  		traits->pbuffer = false;
  		_gc = osg::GraphicsContext::createGraphicsContext(traits.get());
  	}

  	viewer->getCamera()->setGraphicsContext(_gc);
  	viewer->getCamera()->setViewport(0, 0, screenWidth, screenHeight);
  	viewer->getCamera()->setClearColor(osg::Vec4(1, 1, 1, 1));
  	viewer->getCamera()->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);

  	osg::ref_ptr<osg::Hint> persCorrect = new osg::Hint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  	viewer->getCamera()->getOrCreateStateSet()->setAttributeAndModes(persCorrect.get(),	osg::StateAttribute::ON);
  	// viewer->getCamera()->setCullingMode(osg::CullSettings::NO_CULLING);
  	// viewer->getCamera()->getView()->setLightingMode(osg::View::HEADLIGHT);
    // viewer->getCamera()->getView()->setLightingMode(osg::View::SKY_LIGHT);

  	sceneRoot = new osg::Group;
  	for(int modelIndex = 0; modelIndex < loadedModels.size(); modelIndex++){
        sceneRoot->addChild(loadedModels[modelIndex]);
    }
    //---------------------------------------------------------------------
    // all nodes in the scene graph could benefit from the two light sources
    // osg::LightSource* light0 = createLightSource(0, osg::Vec3(-20.0f, 0.0f, 0.0f), osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    // osg::LightSource* light1 = createLightSource(1, osg::Vec3(0.0f, -20.0f, 0.0f), osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    // osg::LightSource* light2 = createLightSource(1, osg::Vec3(0.0f, 0.0f, -20.0f), osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));
    // sceneRoot->getOrCreateStateSet()->setMode( GL_LIGHT0, osg::StateAttribute::ON);
    // sceneRoot->getOrCreateStateSet()->setMode( GL_LIGHT1, osg::StateAttribute::ON);
    // sceneRoot->getOrCreateStateSet()->setMode( GL_LIGHT2, osg::StateAttribute::ON);
    // sceneRoot->addChild(light0);
    // sceneRoot->addChild(light1);
    // sceneRoot->addChild(light2);
    //---------------------------------------------------------------------


  	//	osg::ref_ptr<osg::Light> light = new osg::Light;
  	//	osg::ref_ptr<osg::LightSource> lightSource = new osg::LightSource;
  	//	light->setAmbient(osg::Vec4(1.0,1.0,1.0,1.0));
  	//	light->setDiffuse(osg::Vec4(1.0,1.0,1.0,1.0));
  	//	light->setSpecular(osg::Vec4(1,1,1,1));
  	//	lightSource->setLight(light.get());
  	//	root->addChild(lightSource.get());

  	viewer->setSceneData( sceneRoot.get() );

  	///////////////////////////////////////////////////////
  	//          Set camera 								 //
  	///////////////////////////////////////////////////////

  	for(int modelIndex = 0; modelIndex < loadedModels.size(); modelIndex++){
  	    boundingSpheres.push_back( loadedModels[modelIndex]->getBound() );
      	objectCenters.push_back( boundingSpheres[modelIndex].center() );
      	objectRadiuses.push_back( boundingSpheres[modelIndex].radius() );
    }

    setModelIndex(0);
  	setViewpoint(_azimuth, _elevation, _yaw, _distance, _fieldOfView);
  	viewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
  	viewer->setUpThreading();
  	viewer->realize();
  	if (offScreen){
  		callBack = new CaptureCB(viewer, screenWidth, screenHeight, offScreen);
  		viewer->getCamera()->setFinalDrawCallback(callBack);
  	}
  	return true;
}

// Inefficient method to add a collection of models
bool Renderer::addModel(std::string fileName){

    osgUtil::Optimizer optimizer;
    osgUtil::Optimizer::TextureVisitor tv(true, false, false, false, false, false);
    osgUtil::SmoothingVisitor sv;

    loadedModels.push_back(osgDB::readNodeFile(fileName));
    	// Fail to load object
   	if (!loadedModels.back()){
    		std::cout<<"Cannot load the model : " << fileName << std::endl;
            loadedModels.pop_back();
    		return false;
   	}

    // optimizer.optimize(loadedModels.back().get());
    // loadedModels.back()->accept(tv);
    // Smooth faces
    // loadedModels.back()->accept(sv);

    sceneRoot->addChild(loadedModels.back());
    modelNames.push_back(fileName);
}

void Renderer::render(unsigned char * _rendering, double * _depth){
	if (offScreen){
		callBack->setRendering(_rendering);
		callBack->setDepth(_depth);
		viewer->setDone(false);
		viewer->frame();
		do{
			// std::cout<<"Renderer : Rendering..."<<std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}while(!viewer->done());
	}
	else{
		do{
			viewer->frame();
			std::cout<<azimuth<<" "<<elevation<<" "<<distance<<" "<<fieldOfView<<std::endl;
		}while(!viewer->done());
	}
}

// void Renderer::flipRenderingPy(unsigned char *imageInput, int gWidth , int gHeight, unsigned char*imageOutput)
// {
// 	if (NULL == imageOutput) {
// 		return;
// 	} else {
// 		// Rendered image is flipped.
// 		// Assume that the imageOutput is initialized.
// 		for (int i = 0;i < gHeight;i++) {
// 			unsigned char *rowPtr2 = imageInput + (gHeight - 1 - i) * gWidth * 3;
// 			for (int w = 0;w < gWidth;w++) {
// 				// red
// 				imageOutput[                   w + i*gWidth] = rowPtr2[w*3];
// 				// green
// 				imageOutput[gHeight*gWidth   + w + i*gWidth] = rowPtr2[w*3+1];
// 				// blue
// 				imageOutput[gHeight*gWidth*2 + w + i*gWidth] = rowPtr2[w*3+2];
// 			}
// 		}
// 	}
// }

void Renderer::flipRendering(unsigned char *imageInput, int gWidth , int gHeight, unsigned char*imageOutput)
{
	if (NULL == imageOutput) {
		return;
	} else {
		// Rendered image is flipped.
		// Assume that the imageOutput is initialized.
		for (int i = 0;i < gHeight;i++) {
			unsigned char *rowPtr2 = imageInput + (gHeight - 1 - i) * gWidth * 3;
			for (int w = 0;w < gWidth;w++) {
				// red
				imageOutput[w*gHeight+i] = rowPtr2[w*3];
				// green
				imageOutput[gHeight*gWidth+w*gHeight+i] = rowPtr2[w*3+1];
				// blue
				imageOutput[gHeight*gWidth*2+w*gHeight+i] = rowPtr2[w*3+2];
			}
		}
	}
}

void Renderer::flipDepth(float * depthInput, int gWidth, int gHeight, double * depthOutput){
	for (int i = 0;i < gHeight;i++) {
		for (int j = 0;j < gWidth;j++) {
			depthOutput[i*gWidth+j] = depthInput[(gWidth-1-j)*gHeight+i];
		}
	}
}

} /* namespace CLR */
