startup;

% demo renderer
if exist('renderer','var')
  renderer.delete();
  clear renderer;
end

ldpath = getenv('LD_LIBRARY_PATH');
setenv('LD_LIBRARY_PATH',['/usr/local/lib64/:', ldpath]);

DEBUG = false;
compile;

%%%%%%%%%%% Start making Rendering engine %%%%%%%%%%
renderer = Renderer();
if ~renderer.initialize({'mesh/Honda-Accord.3ds', 'mesh/road_bike.3ds', 'mesh/untitled.dae'},700,700,45,0,0,0,25)
    error('Renderer initilization failed');
end

renderer.setModelIndex(1);
renderer.setViewpoint(0,20,0,0,25);

[rendering, depth]= renderer.render();
subplot(121);imagesc(rendering);
subplot(122);imagesc(depth);

% For matlab version > 2012, measure how long it takes
% timeit(@() renderer.render())

el = 30;
yaw = 0;

% renderer.setViewport(400,400);

renderer.setModelIndex(3);
for az = 0:90:180
  for fov = 15:10:65
    renderer.setViewpoint(az,el,yaw,0,fov);
    [rendering]= renderer.render();
    figure(2); imagesc(rendering); pause(0.002);
  end
end
