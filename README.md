Matlab Offscreen Renderer
=========================

[TOC]


Efficient Matlab Wrapper for Offscreen Rendering engine using [OpenSceneGraph 3](https://github.com/openscenegraph/osg) and [Mexplus](https://github.com/kyamagu/mexplus). The Matlab wrapper holds the C++ object instance and returns rendering whenever the Matlab wrapper object requests it. It is more efficient since you do not load a CAD model every time you render it. 

The Renderer object remains on memory and it contains all the loaded CAD models and returns data to MATLAB directly.

There are two modes for installation. One that does not require OSG installation which is recommended and the one that works without OSG installation.

Install : Standard (Linux/Mac)
------------------

`Note` Prebuilt OSG library only works for Linux. 

1. Install Open Scene Graph from https://github.com/openscenegraph/osg

    ```
    git clone https://github.com/openscenegraph/osg
    cd osg
    make all
    sudo make install
    ```

2. Clone the MatlabRenderer repo

    ```
    git clone https://github.com/chrischoy/MatlabRenderer.git
    ```

3. Go to the `MatlabRenderer` folder and run `compile.m`

Install : Prebuild (Linux Only)
-------------------------------

1. Clone the MatlabRenderer repo

    ```
    git clone https://github.com/chrischoy/MatlabRenderer.git
    ```
    
2. Add compile library path and runtime library path. Note that `LD_LIBRARY_PATH` is for application library search path and `LIBRARY_PATH` is for compiler library search path. If you open new command line session, you must these lines every time unless you add the ld library path to `.bashrc`
    
    ```
    cd MatlabRenderer
    export LIBRARY_PATH=LIBRARY_PATH:path/to/MatlabRenderer/lib/osg/:path/to/MatlabRenderer/lib/boost/
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:path/to/MatlabRenderer/lib/osg:path/to/MatlabRenderer/lib/boost/
    ```
    
3. Run matlab


Other IDE (Eclipse)
-------------------

1. Use OpenSceneGraph https://github.com/openscenegraph/osg to download latest OSG.
2. Eclipse Setting

- Library to include

    ```
    GL, GLU, osg, osgDB, osgGA, osgViewer, osgUtil, boost_program_options
    ```

- If you installed OSG on `/usr/local/lib64` (which is default)
go to `Run Configuration`, add Environment variable `LD_LIBRARY_PATH` and value `/usr/local/lib64`

- Compile
    
    ```
    g++ -o MatlabRenderer MatlabRenderer.cpp -lGL -losg -losgViewer -losgDB -losgGA -losgUtil -lboost_program_options -O3
    ```


Usage
-----

```
% Add binary path
addpath('./bin');

% Initialize the Matlab object.
renderingSizeX = 700; renderingSizeY = 700; % pixels

azimuth = 90; elevation = 10; yaw = 0;
% if you use field of view, set distance to 0
distance = 0; fieldOfView = 25; 


% Setup Renderer
renderer = Renderer();
if ~renderer.initialize({'mesh/2012-VW-beetle-turbo.3ds', ...
        'mesh/Honda-Accord.3ds',...
        'mesh/untitled.dae'},700,700,45,0,0,0,25)
    error('Renderer initilization failed');
end



% If the output is only the rendering, it renders more efficiently
renderer.setModelIndex(1);
renderer.setViewpoint(azimuth,elevation,yaw,0,fieldOfView);
[rendering]= renderer.render();
subplot(221);
imagesc(rendering); axis equal; axis tight; axis off;


% Once you initialized the renderer, you can just set 
% the viewpoint and render without loading CAD model again.
renderer.setModelIndex(2);
renderer.setViewpoint(azimuth,elevation,yaw,0,fieldOfView);
[rendering]= renderer.render();
subplot(222);
imagesc(rendering); axis equal; axis tight; axis off;



% If you give the second output, it renders depth too.
renderer.setModelIndex(1);
renderer.setViewpoint(45,20,0,0,25);

[rendering, depth]= renderer.render();
subplot(223);imagesc(rendering); axis equal; axis off;
subplot(224);imagesc(1-depth); axis equal; axis off; colormap hot;


% You must clear the memory before you exit
renderer.delete(); clear renderer;
```

Example 

![](https://dl.dropboxusercontent.com/u/57360783/MatlabRenderer/rendering_with_depth.png)


Input CAD format
----------------

List of available formats 
http://trac.openscenegraph.org/projects/osg//wiki/Support/UserGuides/Plugins

To get free models, use [Google 3D Warehouse](https://3dwarehouse.sketchup.com) and use Sketchup to edit models.
Rotate the model and export it to the format that you will use.

COLLADA (DAE) format support
-------------
Follow the direction of
https://github.com/openscenegraph/osg/tree/master/src/osgPlugins/dae

Specifically, 
http://collada.org/mediawiki/index.php/DOM_guide:_Setting_up

1. download COLLADA from

	>    svn co https://collada-dom.svn.sourceforge.net/svnroot/collada-dom/trunk colladadom

2. compile COLLADA

	> cd colladadom
	> ccmake .

	and configure and generate `Makefile`. and

	> make

2. download openscenegraph 

	> cd /to/your/osg/directory/
	> git clone https://github.com/openscenegraph/osg

3. go to the downloaded `osg` and `ccmake` the folder

	> cd osg
	> ccmake  .

	and configure and make sure that COLLADA_DOM_ROOT, COLLADA_BOOST_FILESYSTEM_LIBRARY, COLLADA_DYNAMIC_LIBRARY,  COLLADA_INCLUDE_DIR have been set correctly.
	
	COLLADA_DOM_ROOT :
	COLLADA_BOOST_FILESYSTEM_LIBRARY : Boost library. something like /usr/local/lib/libboost_filesystem.dylib
	COLLADA_BOOST_SYSTEM_LIBRARY : Boost library. something like /usr/local/lib/libboost_system.dylib
	COLLADA_DYNAMIC_LIBRARY : /path/to/colladadom/dom/libcollada-dom2.4-dp.dylib
	COLLADA_INCLUDE_DIR : /path/to/colladadom/dom/include where dae.h resides
	
	> make


The precompiled library contains osgPlugin for collada format. (LINUX)


ISSUE
----------------

### Q: Warning: could not find plugin to read objects from file *.*

A: The reason that it fails to load is that the renderer fails to load appropriate shared library. In the prebuilt ./OSG/lib/osgPlugins-3.3.2, yoou can see several plugins. Add the plugin path to 'LD_L
IBRARY_PATH`.

You must either install complete OSG package or download from http://trac.openscenegraph.org/projects/osg//wiki/Downloads/Dependencies

### Q: Your MEX-file “path/MatlabRenderer/Renderer_mexa64” is locked and must be unlocked before recompiling

The binary file is in use. Restart the matlab which will clear the memory.


TODO
----

1. Windows installation
2. Add dylib for prebuilt OS X

- http://stackoverflow.com/questions/3146274/is-it-ok-to-use-dyld-library-path-on-mac-os-x-and-whats-the-dynamic-library-s

> Written with [StackEdit](https://stackedit.io/).
