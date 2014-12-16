CC = gcc
CFLAGS 		 = -std=c++11 -fPIC -Wall
LDFLAGS      = -shared -Wl,--no-allow-shlib-undefined 
RELEASEFLAGS = -O2 -D NDEBUG


# Run numpy.get_include() in python to get the following path
NUMPY_INCLUDE = /home/chrischoy/anaconda/lib/python2.7/site-packages/numpy/core/include
# PYCONFIG_PATH = /usr/local/python2.7/
PYCONFIG_PATH = /home/chrischoy/anaconda/include/python2.7


TARGET   = PyRenderer.so
SOURCES  = src/Renderer.cpp src/PyRenderer.cpp
INCLUDES = -I./src -I$(PYCONFIG_PATH) -I$(NUMPY_INCLUDE) 
LIBS     = -lGL -lGLU -losg -losgDB -losgGA -losgViewer -losgUtil -lstdc++ -lm -lboost_python -lpython2.7

all: clean
	$(CC) $(CFLAGS) $(LDFLAGS) $(RELEASEFLAGS) $(SOURCES) -o $(TARGET) $(INCLUDES) $(LIBS)

clean:
	rm -f $(TARGET)
