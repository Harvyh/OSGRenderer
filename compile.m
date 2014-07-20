% If the compile fails, add  '-std=c++11' at the end of CFLAG and CXXFLAGS
% inside mexopts.sh. 
% If you don't know where `mexopts.sh` is, type `mex -v -n` and read the
% output carefully. 
% 
% 1. change mexopts.sh and add '-std=c++11' to CXXFLAGS
% 2. make('all','-lGL -lGLU -losg -losgDB -losgGA -losgViewer -losgUtil');

if isunix && ~ismac
  system('rm Renderer_.mexa64');
  make('all','-lGL -lGLU -losg -losgDB -losgGA -losgViewer -losgUtil');
end

% For mac
%
% 1. add -framework OpenGL
% 
% TMW_ROOT="$MATLAB"
%     MFLAGS=''
%     if [ "$ENTRYPOINT" = "mexLibrary" ]; then
%         MLIBS="-L$TMW_ROOT/bin/$Arch -lmx -lmex -lmat -lmwservices -lut -framework OpenGL"
%     else  
%         MLIBS="-L$TMW_ROOT/bin/$Arch -lmx -lmex -lmat  -framework OpenGL"
%     fi
%     case "$Arch" in
%         Undetermined)
%         
% 2. change mexopts.sh and add '-std=c++11' to CXXFLAGS
% 3. make('all','-I/usr/local/include -losg -losgDB -losgGA -losgViewer -losgUtil -lOpenThreads');

if ismac
  system('rm Renderer_.mexmaci64');
  make('all','-I/usr/local/include -L/usr/local/lib -losg -losgViewer -losgDB -losgGA -losgUtil -lOpenThreads -lOpenThreads');
end