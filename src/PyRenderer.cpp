/*
 * Renderer.cpp
 *
 *  Created on: Jul 3, 2014
 *      Author: Christopher B. Choy
 */
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "Renderer.hpp"

using namespace boost::python;

BOOST_PYTHON_MODULE(PyRenderer)
{
  class_<CLR::Renderer>("Renderer")
    .def("initialize", &CLR::Renderer::initialize)
    .def("setModelIndex", &CLR::Renderer::setModelIndex)
    .def("setViewpoint", &CLR::Renderer::setViewpoint)
  ;
}

/* namespace CLR */
