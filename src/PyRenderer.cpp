/*
 * Renderer.cpp
 *
 *  Created on: Jul 3, 2014
 *      Author: Christopher B. Choy
 */

#include <boost/python.hpp>
#include <boost/python/raw_function.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

// To return rendered images and depth, 
#include <numpy/ndarrayobject.h>

#include "Renderer.hpp"

namespace py=boost::python;

// template< typename T >
// inline
// std::vector< T > to_std_vector( const py::object& iterable )
// {
//       return std::vector< T >( py::stl_input_iterator< T >( iterable ),
//                                py::stl_input_iterator< T >( ) );
// }
//
py::object initialize_wrapper(py::tuple args, py::dict kwargs)
{
    CLR::Renderer& self = py::extract<CLR::Renderer&>(args[0]);
   
    // Arguments are strve fileNames,
    //		bool _offScreen,
    //		int _screenWidth,
    //		int _screenHeight,
    //		double _azimuth = 0,
    //		double _elevation = 0,
    //		double _yaw = 0,
    //		double _distance = 100,
    //		double _fieldOfView = 25e
    //py::list keys = kwargs.keys();
   
    py::list fileNamesList = py::extract<py::list>(args[1]);
    strvec fileNamesVector;
    for(int i = 0; i < py::len(fileNamesList); ++i)
        fileNamesVector.push_back(py::extract<std::string>(fileNamesList[i]));
    const bool offScreen    = true;
    int screenWidth   = py::extract<int>(args[2]);
    int screenHeight  = py::extract<int>(args[3]);
    double az         = py::extract<double>(args[4]);
    double el         = py::extract<double>(args[5]);
    double yaw        = py::extract<double>(args[6]);
    double dist       = py::extract<double>(args[7]);
    double fov        = py::extract<double>(args[8]);

    self.initialize(fileNamesVector, offScreen, screenWidth, screenHeight, az, el, yaw, dist, fov);

    py::numeric::array::set_module_and_type("numpy", "ndarray");

    return py::object();
}

py::object render_wrapper(py::tuple args, py::dict kwargs)
{
    CLR::Renderer& self = py::extract<CLR::Renderer&>(args[0]);
     
    int depth_size      = self.screenWidth * self.screenHeight;
    int rendering_size  = depth_size * 3;
    
    npy_intp depth_size_npy[]     = {self.screenWidth, self.screenHeight}; 
    npy_intp rendering_size_npy[] = {3, self.screenWidth, self.screenHeight}; 
    
    // Initialize rendering
    std::vector<unsigned char> rendering(rendering_size);
    std::vector<double> depth(depth_size);

    /* const_cast is rather horrible but we need a writable pointer
     *         in C++11, vec.data() will do the trick
     *                 but you will still need to const_cast
     */
    unsigned char * ptr_rendering = rendering_size ? const_cast<unsigned char *>(&rendering[0]) 
                                                   : static_cast<unsigned char *>(NULL); 
    double * ptr_depth = depth_size ? const_cast<double *>(&depth[0]) 
                                    : static_cast<double *>(NULL); 

    // Render rendering and depth
    self.render(ptr_rendering, ptr_depth);
    
    // create a PyObject * from pointer and data 
    PyObject * pyRendering = PyArray_SimpleNewFromData( 3, &rendering_size_npy[0], NPY_UBYTE, ptr_rendering );
    py::handle<> hRendering( pyRendering );
    py::numeric::array aRendering( hRendering );
   
    PyObject * pyDepth = PyArray_SimpleNewFromData( 2, &depth_size_npy[0], NPY_FLOAT32, ptr_depth );
    py::handle<> hDepth( pyDepth );
    py::numeric::array aDepth( hDepth );
 
    /* The problem of returning arr is twofold: firstly the user can modify
     * the data which will betray the const-correctness 
     * Secondly the lifetime of the data is managed by the C++ API and not the 
     * lifetime of the numpy array whatsoever. But we have a simple solution..
     */
    //  return py::object(hRendering); // copy the object. numpy owns the copy now.
    // return aRendering.copy(); // copy the object. numpy owns the copy now.
    return py::make_tuple(aRendering.copy(), aDepth.copy()); // copy the object. numpy owns the copy now.
}

// py::object stdVecToNumpyArray( std::vector<double> const& vec )
// {
//     npy_intp size = vec.size();
// 
//     /* const_cast is rather horrible but we need a writable pointer
//      *         in C++11, vec.data() will do the trick
//      *                 but you will still need to const_cast
//      *                       */
// 
//     double * data = size ? const_cast<double *>(&vec[0]) 
//                         : static_cast<double *>(NULL); 
// 
//     // create a PyObject * from pointer and data 
//     PyObject * pyObj = PyArray_SimpleNewFromData( 1, &size, NPY_DOUBLE, data );
//     boost::python::handle<> handle( pyObj );
//     boost::python::numeric::array arr( handle );
//     
//     /* The problem of returning arr is twofold: firstly the user can modify
//        the data which will betray the const-correctness 
//        Secondly the lifetime of the data is managed by the C++ API and not the 
//        lifetime of the numpy array whatsoever. But we have a simple solution..
//     */
//     
//     return arr.copy(); // copy the object. numpy owns the copy now.
// }

BOOST_PYTHON_MODULE(PyRenderer)
{
//  py::class_<strvec>("strvec")
//    .def( py::vector_indexing_suite<strvec>() )
//  ;

  py::class_<CLR::Renderer>("Renderer")
    .def("initialize"        , py::raw_function(&initialize_wrapper,4))
    .def("render"            , py::raw_function(&render_wrapper,1))
    .def("setModelIndex"      , &CLR::Renderer::setModelIndex)
    .def("setViewpoint"       , &CLR::Renderer::setViewpoint)
  ;

  import_array();
}

/* namespace CLR */
