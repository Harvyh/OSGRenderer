//============================================================================
// Name        : RendererWrapper.cpp
// Author      : Christopher B. Choy
// Version     : 0.1
// Copyright   :
// Description : MATLAB Renderer
//============================================================================

#include <boost/program_options.hpp>
#include "Renderer.h"

namespace po = boost::program_options;
using namespace std;

int main(int ac , char **av){

	string filename;
	double azimuth = 0, elevation = 0, yaw = 0, distance = 0, fieldOfView = 25;
	int screenWidth = 400, screenHeight = 400;
	bool offScreen = false;

	try {
		po::options_description desc("Allowed options");
		desc.add_options()
			("help", "produce help message")
			("model", po::value<string>(), "file name of the model to be displayed")
			("azimuth", po::value<double>(), "Azimuth")
			("elevation", po::value<double>(), "Elevation")
			("yaw", po::value<double>(), "Yaw")
			("distance", po::value<double>(), "Distance")
			("fieldOfView", po::value<double>(), "Field of view")
			("screenWidth", po::value<int>(), "Screen width")
			("screenHeight", po::value<int>(), "Screen height")
			("offScreen", po::value<bool>(), "Off Screen")
		;

		po::variables_map vm;
		po::store(po::parse_command_line(ac, av, desc), vm);
		po::notify(vm);

		if (vm.count("help")) {
			cout << desc << "\n";
			return 0;
		}

		if (vm.count("model")) {
			filename = vm["model"].as<string>();
			cout << "model file is " << filename << ".\n";
		} else {
			cout << "model file not set.\n";
			return 0;
		}
		if (vm.count("azimuth")){
			azimuth = vm["azimuth"].as<double>();
		}
		if (vm.count("elevation")){
			elevation = vm["elevation"].as<double>();
		}
		if (vm.count("yaw")){
			yaw= vm["yaw"].as<double>();
		}
		if (vm.count("distance")){
			distance= vm["distance"].as<double>();
		}
		if (vm.count("fieldOfView")){
			fieldOfView = vm["fieldOfView"].as<double>();
		}
		if (vm.count("screenWidth")){
			screenWidth = vm["screenWidth"].as<int>();
		}
		if (vm.count("screenHeight")){
			screenHeight = vm["screenHeight"].as<int>();
		}
		if (vm.count("offScreen")){
			offScreen = vm["offScreen"].as<bool>();
		}
	}
	catch(exception& e) {
		cerr << "error: " << e.what() << "\n";
		return 1;
	}

	CLR::Renderer* renderer = new CLR::Renderer();
	bool succeed = renderer->initialize(filename, offScreen, screenWidth, screenHeight, azimuth, elevation, yaw, distance, fieldOfView);
    if (succeed) renderer->render((unsigned char *)NULL, (double *)NULL);
	// Renderer::copyRendering(GLubyte *imageInput, int gWidth , int gHeight, GLubyte *imageOutput);

	return 1;
}

