# gcc -c -std=c++11 -fPIC src/Renderer.cpp -o lib/_Renderer.o
# gcc -c -std=c++11 -fPIC src/PyRenderer.cpp -o lib/_PyRenderer.o -I/usr/include/python2.7/
# gcc -std=c++11 -fPIC -shared -Wl,-soname,Renderer.so -o lib/Renderer.so lib/_Renderer.o lib/_PyRenderer.o -lboost_python -lpython2.7 
# 
# 
gcc -std=c++11 -Wall -fPIC -shared -Wl,--no-allow-shlib-undefined src/Renderer.cpp src/PyRenderer.cpp -I./src -o lib/PyRenderer.so -lboost_python -lpython2.7 -lGL -lGLU -losg -losgDB -losgGA -losgViewer -losgUtil -lstdc++ -lm -I/usr/include/python2.7/
