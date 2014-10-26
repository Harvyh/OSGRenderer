startup;

% demo renderer
if exist('renderer','var')
  renderer.delete();
  clear renderer;
end

DEBUG = false;
compile;

%%%%%%%%%%% Start making Rendering engine %%%%%%%%%%
renderer = Renderer();
if ~renderer.initialize({'mesh/Honda-Accord.3ds',...
        'mesh/road_bike.3ds', 'mesh/untitled.dae'},700,700,45,0,0,0,25)
    error('Renderer initilization failed');
end

renderer.setModelIndex(1);
renderer.setViewpoint(0,20,0,0,25);

[rendering, depth]= renderer.render();
subplot(121);imagesc(rendering); axis equal; axis off;
subplot(122);imagesc(1-depth); axis equal; axis off; colormap hot;

% For matlab version > 2012, measure how long it takes
timeit(@() renderer.render())

% renderer.setViewport(400,400);

renderer.setModelIndex(3);
for az = 0:90:180
  for fov = 15:10:65
    renderer.setViewpoint(az,30,0,0,fov);
    [rendering]= renderer.render();
    figure(2); imagesc(rendering); pause(0.002);
  end
end
