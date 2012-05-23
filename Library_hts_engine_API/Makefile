
TARGET   = lib/hts_engine_API.a

SOURCES  = src/lib/HTS_audio.c \
           src/lib/HTS_engine.c \
           src/lib/HTS_gstream.c \
           src/lib/HTS_label.c \
           src/lib/HTS_misc.c \
           src/lib/HTS_model.c \
           src/lib/HTS_pstream.c \
           src/lib/HTS_sstream.c \
           src/lib/HTS_vocoder.c

OBJECTS  = $(SOURCES:.c=.o)

CXX      = gcc
AR       = ar
CXXFLAGS = -Wall -O6 -fomit-frame-pointer \
           -DAUDIO_PLAY_PORTAUDIO
INCLUDE  = -I ../Library_PortAudio/include \
           -I src/include

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) cru $(TARGET) $(OBJECTS)
	cp src/include/HTS_engine.h include/HTS_engine.h

.c.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(<:.c=.o) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)
