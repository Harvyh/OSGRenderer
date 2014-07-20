classdef Renderer < handle
%Renderer : MATLAB wrapper of openscenegraph rendering engine.
%
% This class definition gives an interface to the underlying MEX functions
% built in the private directory. It is a good practice to wrap MEX functions
% with Matlab script so that the API is well documented and separated from
% its C++ implementation. Also such a wrapper is a good place to validate
% input arguments.
%
% Build
% -----
%
%    make
%
% See `make.m` for details.
%

properties (Access = private)
  id_ % ID of the session.
end

methods
  function this = Renderer()
  %DATABASE Create a new database.
    this.id_ = Renderer_('new');
  end

  function delete(this)
  %DELETE Destructor.
    Renderer_('delete', this.id_);
  end

  function initialize(this, filename, screenWidth, screenHeight, azimuth, elevation, yaw, distance, fieldOfView)
  %PUT Save something to the database.
    assert(isscalar(this));
    offScreen = 1;
    Renderer_('initialize', this.id_, filename, offScreen, screenWidth, screenHeight, azimuth, elevation, yaw, distance, fieldOfView);
  end
  
  function setViewport(this, screenWidth, screenHeight)
  %PUT Save something to the database.
    assert(isscalar(this));
    Renderer_('setViewport', this.id_, screenWidth, screenHeight);
  end
  
  function setViewpoint(this, az, el, yaw, dist, fov)
  %PUT Save something to the database.
    assert(isscalar(this));
    Renderer_('setViewpoint', this.id_, az, el, yaw, dist, fov);
  end
  
  function [rendering, depth] = render(this)
  %QUERY Query something to the database.
    assert(isscalar(this));
    if nargout == 1
      rendering = Renderer_('render', this.id_);
    else
      [rendering, depth] = Renderer_('render', this.id_);
    end
  end
end

end