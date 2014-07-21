Matlab Renderer
==============

[TOC]


Efficient Matlab Rendering engine using [OpenSceneGraph 3](https://github.com/openscenegraph/osg) and [Mexplus](https://github.com/kyamagu/mexplus). The Matlab wrapper holds the C++ object instance and returns rendering whenever the Matlab wrapper object requests it. It is more since you do not load a CAD model every time you render it. 

There are two modes for installation. One that does not require OSG installation which is recommended and the one that works without OSG installation.


Install : Standard (Linux/Mac)
------------------

`Note` Prebuilt OSG library only works in Linux. If you want to use 

1. Install Open Scene Graph from https://github.com/openscenegraph/osg

    ```
    git clone https://github.com/openscenegraph/osg
    cd osg
    make all
    sudo make install
    ```

2. Locate `mexopts.sh` file which is used for compiling mex file. 
    - Type `mex -v -n` in the MATLAB command line
    - Read the output and locate `mexopts.sh` file. There are two paths: 1. your Matlab default file 2. your current configuration file. You must edit the current configuration file.

3. Add `-std=c++11` to CXXFLAGS in `mexopts.sh`
    - For Mac
        - add line `CFLAGS="$CFLAGS -std=c++11"
    - For Linux
        - if you are using G++ version < 4.7 add line `CFLAGS="$CFLAGS -std=c++0x"` 
        - if you are using G++ version >= 4.7 add line `CFLAGS="$CFLAGS -std=c++11"`

4. **Mac only** add `-framework OpenGL` to `mexopts.sh`. In Mac, to use OpenGL, you must add `-framework OpenGL` but `-framework` option is not supported by `mex` 

    ```
    TMW_ROOT="$MATLAB"
    MFLAGS=''
    if [ "$ENTRYPOINT" = "mexLibrary" ]; then
        MLIBS="-L$TMW_ROOT/bin/$Arch -lmx -lmex -lmat -lmwservices -lut -framework OpenGL"
    else  
        MLIBS="-L$TMW_ROOT/bin/$Arch -lmx -lmex -lmat  -framework OpenGL"
    fi
    ```

5. Clone the MatlabRenderer repo

    ```
    git clone https://github.com/chrischoy208/MatlabRenderer.git
    ```

6. Go to the `MatlabRenderer` folder and run `compile.m`

Install : Prebuild (Linux Only)
-------------------------------

1. Clone the MatlabRenderer repo

    ```
    git clone https://github.com/chrischoy208/MatlabRenderer.git
    ```
    
2. Add compile library path and runtime library path. Note that `LD_LIBRARY_PATH` is for application library search path and `LIBRARY_PATH` is for compiler library search path. If you open new command line session, you must these lines every time unless you add the ld library path to `.bashrc`
    
    ```
    cd MatlabRenderer
    export LIBRARY_PATH=LIBRARY_PATH:./OSG/lib
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./OSG/lib
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

- If you installed OSG on `/usr/local/lib` (which is default)
go to `Run Configuration`, add Environment variable `LD_LIBRARY_PATH` and value `/usr/local/lib64`

- Compile
    
    ```
    g++ -o MatlabRenderer MatlabRenderer.cpp -lGL -losg -losgViewer -losgDB -losgGA -losgUtil -lboost_program_options -O3
    ```


Usage
-----

```
% Initialize the Matlab object.
renderer = Renderer();
renderingSizeX = 700 % pixels
renderingSizeY = 700 % pixels
azimuth = 90;
elevation = 45;
yaw = 0;
distance = 0;
fieldOfView = 25;
renderer.initialize('Honda-Accord.3ds',...
    renderingSizeX,...
    renderingSizeY,...
    azimuth,...
    elevation,...
    yaw,...
    distance, 
    fieldOfView);

% If the output is only the rendering, it renders more efficiently
[rendering]= renderer.render();

% Once you initialize, you can just set the viewpoint and render without loading CAD model again.
renderer.setViewpoint(az,el,yaw,0,fov);
[rendering]= renderer.render();

% If you give the second output, it renders depth too.
[rendering, depth]= renderer.render();
subplot(121);imagesc(rendering);
subplot(122);imagesc(depth);

% You must clear the memory before you exit
renderer.delete();
```

Example 

![](https://dl.dropboxusercontent.com/u/57360783/MatlabRenderer/rendering.png)

> Written with [StackEdit](https://stackedit.io/).