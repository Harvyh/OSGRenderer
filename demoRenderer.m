% demo renderer
if exist('renderer','var')
  renderer.delete();
end
clear;

DEBUG = true;
compile;

%%%%%%%%%%% Start making Rendering engine %%%%%%%%%%
renderer = Renderer();
if ~renderer.initialize({'Honda-Accord.3ds', 'road_bike.3ds'},700,700,45,0,0,0,25)
    error('Renderer initilization failed');
end

renderer.setModelIndex(0);
[rendering, depth]= renderer.render();
subplot(121);imagesc(rendering);
subplot(122);imagesc(depth);

% For matlab version > 2012, measure how long it takes
% timeit(@() renderer.render())

el = 30;
yaw = 0;

% renderer.setViewport(400,400);

renderer.setModelIndex(1);
for az = 0:180
  for fov = 15:10:65
    renderer.setViewpoint(az,el,yaw,0,fov);
    [rendering]= renderer.render();
    figure(2); imagesc(rendering); pause(0.002);
  end
end
