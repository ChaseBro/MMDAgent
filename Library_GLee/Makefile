
TARGET   = lib/GLee.a

SOURCES  = src/GLee.c

OBJECTS  = $(SOURCES:.c=.o)

CXX      = gcc
AR       = ar
CXXFLAGS = -Wall -O6 -fomit-frame-pointer
INCLUDE  = -I src

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) cru $(TARGET) $(OBJECTS)
	cp src/GLee.h include/GLee.h

.c.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(<:.c=.o) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)
