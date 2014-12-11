/*
 * Renderer.h
 *
 *  Created on: Jul 3, 2014
 *      Author: Christopher B. Choy
 */

#include <mexplus.h>
#include <mex.h>
#include "Renderer.hpp"

using namespace mexplus;

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

MEX_DEFINE(getViewMatrix) (int nlhs, mxArray* plhs[],
                   int nrhs, const mxArray* prhs[]) {
	InputArguments input(nrhs, prhs, 1);
	OutputArguments output(nlhs, plhs, 1);

	CLR::Renderer* renderer = Session<CLR::Renderer>::get(input.get(0));
	const osg::Matrixd& view_mat = renderer->getViewMatrix();

	plhs[0] = mxCreateDoubleMatrix(4, 4, mxREAL);
	double* mat_raw = mxGetPr(plhs[0]);

    for(int row_idx = 0; row_idx < 4; row_idx ++)
      for(int col_idx = 0; col_idx < 4; col_idx ++){
        mat_raw[row_idx + col_idx * 4] = view_mat(row_idx,col_idx);
      }
	output.set(0,plhs[0]);
}

MEX_DEFINE(getProjectionMatrix) (int nlhs, mxArray* plhs[],
                   int nrhs, const mxArray* prhs[]) {
	InputArguments input(nrhs, prhs, 1);
	OutputArguments output(nlhs, plhs, 1);

	CLR::Renderer* renderer = Session<CLR::Renderer>::get(input.get(0));
	const osg::Matrixd& view_mat = renderer->getProjectionMatrix();

	plhs[0] = mxCreateDoubleMatrix(4, 4, mxREAL);
	double* mat_raw = mxGetPr(plhs[0]);

    for(int row_idx = 0; row_idx < 4; row_idx ++)
      for(int col_idx = 0; col_idx < 4; col_idx ++){
        mat_raw[row_idx + col_idx * 4] = view_mat(row_idx,col_idx);
      }
	output.set(0,plhs[0]);
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