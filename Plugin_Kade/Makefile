
TARGET   = ../Release/Plugins/Plugin_Kade.so

SOURCES  = Kade_Thread.cpp \
           Plugin_Kade.cpp

OBJECTS  = $(SOURCES:.cpp=.o)

LDADD    = ../Library_MMDAgent/lib/MMDAgent.a \
           ../Library_MMDFiles/lib/MMDFiles.a \
           ../Library_GLFW/lib/GLFW.a \
           ../Library_GLee/lib/GLee.a \
           ../Library_Bullet_Physics/lib/Bullet_Physics.a \
           ../Library_JPEG/lib/JPEG.a\
           ../Library_libpng/lib/libpng.a

CXX      = gcc
AR       = ar
CXXFLAGS = -Wall -O6 -fomit-frame-pointer \
           -shared -g \
           -DMMDAGENT
INCLUDE  = -I ../Library_Bullet_Physics/include \
           -I ../Library_GLee/include \
           -I ../Library_GLFW/include \
           -I ../Library_MMDFiles/include \
           -I ../Library_MMDAgent/include \
           -I /usr/include/python2.7

all: $(TARGET)

$(TARGET): $(OBJECTS) $(LDADD)
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LDADD) $(LADD) -o $(TARGET) \
	-lGLU -lGL -lX11 -lasound -ldl -lrt -lm -lpthread -D_REENTRANT  -lpulse-simple -lpulse -lpython2.7 -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(<:.cpp=.o) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)
