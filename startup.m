global G_STARTUP_RENDERER;

if isempty(G_STARTUP_RENDERER)
  G_STARTUP_RENDERER = true;
    incl = {'bin'};
  for i = 1:length(incl)
    addpath(genpath(incl{i}));
  end
end
