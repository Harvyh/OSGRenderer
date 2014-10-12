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
  %Renderer Create a new database.
    this.id_ = Renderer_('new');
  end

  function delete(this)
  %DELETE Destructor.
    Renderer_('delete', this.id_);
  end

  function success = initialize(this, filenames, screenWidth, screenHeight, azimuth, elevation, yaw, distance, fieldOfView)
  %Initialize initialize renderer.
    assert(isscalar(this));
    assert(~isempty(filenames));
    offScreen = 1;
    success = Renderer_('initialize', this.id_, filenames, offScreen, screenWidth, screenHeight, azimuth, elevation, yaw, distance, fieldOfView);
    if (~success)
        disp('Fail to initialize');
    end
  end
  
  % Setting viewport is not supported once the viewport is created.
  % function setViewport(this, screenWidth, screenHeight)
  % %SetViewport .
  %   assert(isscalar(this));
  %   Renderer_('setViewport', this.id_, screenWidth, screenHeight);
  % end
  
  function setViewpoint(this, az, el, yaw, dist, fov)
  %setViewpoint set azimuth elevation yaw distance and field of view (view angle)
    assert(isscalar(this));
    Renderer_('setViewpoint', this.id_, az, el, yaw, dist, fov);
  end

  % Matlab index base 1 to C++ base 0 indexing
  function setModelIndex(this, modelIndex)
    assert(isscalar(this))
    Renderer_('setModelIndex', this.id_, modelIndex-1);
  end
  
  function [rendering, depth] = render(this)
  %QUERY Query something to the database.
    assert(isscalar(this));
    if nargout > 1
      [rendering, depth] = Renderer_('render', this.id_);
    else
      rendering = Renderer_('render', this.id_);
    end
  end

  function [cropRendering, cropDepth] = renderCrop(this)
  % render image and crop tightly
    assert(isscalar(this));
    [rendering, depth] = Renderer_('render', this.id_);
    rowIdx = find(sum(depth,1) > 0);
    colIdx = find(sum(depth,2) > 0);
    minRowI = min(rowIdx);
    maxRowI = max(rowIdx);
    minColI = min(colIdx);
    maxColI = max(colIdx);

    x1 = minRowI;
    x2 = maxRowI;
    y1 = minColI;
    y2 = maxColI;

    cropRendering = rendering(y1:y2,x1:x2,:);
    if nargout > 1
      cropDepth = depth(y1:y2,x1:x2,:);
    end
  end
end
end
