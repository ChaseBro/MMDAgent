
TARGET   = lib/libpng.a

SOURCES  = src/png.c \
           src/pngerror.c \
           src/pngget.c \
           src/pngmem.c \
           src/pngpread.c \
           src/pngread.c \
           src/pngrio.c \
           src/pngrtran.c \
           src/pngrutil.c \
           src/pngset.c \
           src/pngtrans.c \
           src/pngwio.c \
           src/pngwrite.c \
           src/pngwtran.c \
           src/pngwutil.c

OBJECTS  = $(SOURCES:.c=.o)

CXX      = gcc
AR       = ar
CXXFLAGS = -Wall -O6 -fomit-frame-pointer
INCLUDE  = -I ../Library_zlib/include \
           -I src

all: $(TARGET)

$(TARGET): src/pnglibconf.h $(OBJECTS)
	$(AR) cru $(TARGET) $(OBJECTS)
	cp src/*.h include

src/pnglibconf.h:
	cp src/scripts/pnglibconf.h.prebuilt src/pnglibconf.h

.c.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(<:.c=.o) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -f src/pnglibconf.h
