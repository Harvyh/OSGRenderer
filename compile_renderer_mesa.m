function compile_renderer_mesa(MESA)                                                                                                                                                    
OSG_LIB_PATH = 'lib/osg';
OSG_INC_PATH = 'include/osg';
MESA_LIB_PATH = 'lib/mesa';
MESA_INC_PATH = 'include/mesa';

cmd='mex  Renderer.cpp -output ''bin/Renderer_'' -v -lGL -losg -losgViewer -losgDB -losgGA -losgUtil -lOpenThreads -lGLU -Iinclude';

if ismac
  cmd = [ cmd ' CXXFLAGS=''$CXXFLAGS -stdlib=libc++ -std=gnu++11''' ];
end

if exist('OSG_LIB_PATH','var')
    cmd=[cmd sprintf(' -L%s -L%s -I%s -I%s',OSG_LIB_PATH,MESA_LIB_PATH,OSG_INC_PATH,MESA_INC_PATH)];
end

fprintf('Executing %s\n',cmd);
eval(cmd);
