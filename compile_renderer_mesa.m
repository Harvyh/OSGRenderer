function compile_renderer_mesa(OSG_PATH, MESA_PATH)
OSG_PATH = 'OSG';
MESA_PATH = 'Mesa-7.0.3';

cmd='mex Renderer.cpp -v -lGL -losg -losgViewer -losgDB -losgGA -losgUtil -lOpenThreads -lGLU -Iinclude';

if ismac
  cmd = [ cmd ' CXXFLAGS=''$CXXFLAGS -stdlib=libc++ -std=gnu++11''' ];
end

if exist('OSG_PATH','var') && ~isempty(OSG_PATH)
    libpath=fullfile(OSG_PATH,'lib');
    lib64path=fullfile(OSG_PATH,'lib');
    incpath=fullfile(OSG_PATH,'include');
    cmd=[cmd sprintf(' -L%s -L%s -I%s',libpath,lib64path,incpath)];
end

if exist('MESA_PATH','var') && ~isempty(MESA_PATH)
    % GLPATH='Mesa-7.0.3';
    GLincpath=fullfile(MESA_PATH,'include');
    GLlibpath=fullfile(MESA_PATH,'lib64');
    cmd=[cmd sprintf(' -L%s -I%s',GLlibpath,GLincpath)];
end

fprintf('Executing %s\n',cmd);
eval(cmd);
