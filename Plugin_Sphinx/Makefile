
TARGET   = ../Release/Plugins/Plugin_Sphinx.so

SOURCES  = Sphinx.cpp \
           Sphinx_Thread.cpp \
           Plugin_Sphinx.cpp

OBJECTS  = $(SOURCES:.cpp=.o)

LDADD    = ../Library_MMDAgent/lib/MMDAgent.a \
           ../Library_MMDFiles/lib/MMDFiles.a \
           ../Library_GLFW/lib/GLFW.a \
           ../Library_GLee/lib/GLee.a \
           ../Library_Bullet_Physics/lib/Bullet_Physics.a \
           ../Library_JPEG/lib/JPEG.a \
           /usr/local/lib/libpocketsphinx.a \
           /usr/local/lib/libsphinxbase.a \
           /usr/local/lib/libsphinxad.a

CXX      = gcc
AR       = ar
CXXFLAGS = -Wall -O0 -fomit-frame-pointer \
           -shared -ggdb\
           -DMMDAGENT
INCLUDE  = -I ../Library_Bullet_Physics/include \
           -I ../Library_GLee/include \
           -I ../Library_GLFW/include \
           -I ../Library_MMDFiles/include \
           -I ../Library_MMDAgent/include \
           -I /usr/local/include/pocketsphinx \
           -I /usr/local/include/sphinxbase

all: $(TARGET)

$(TARGET): $(OBJECTS) $(LDADD)
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LDADD) $(LADD) -o $(TARGET) \
	-lGLU -lGL -lX11 -lasound -ldl -lrt -lm -lpthread -D_REENTRANT  -lpulse-simple -lpulse -lpng12

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(<:.cpp=.o) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)
