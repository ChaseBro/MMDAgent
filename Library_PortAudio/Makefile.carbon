
TARGET   = lib/PortAudio.a

SOURCES  = src/src/common/pa_allocation.c \
           src/src/common/pa_converters.c \
           src/src/common/pa_cpuload.c \
           src/src/common/pa_debugprint.c \
           src/src/common/pa_dither.c \
           src/src/common/pa_front.c \
           src/src/common/pa_process.c \
           src/src/common/pa_ringbuffer.c \
           src/src/common/pa_skeleton.c \
           src/src/common/pa_stream.c \
           src/src/common/pa_trace.c \
           src/src/hostapi/coreaudio/pa_mac_core.c \
           src/src/hostapi/coreaudio/pa_mac_core_blocking.c \
           src/src/hostapi/coreaudio/pa_mac_core_utilities.c \
           src/src/os/mac_osx/pa_mac_hostapis.c \
           src/src/os/unix/pa_unix_util.c

OBJECTS  = $(SOURCES:.c=.o)

CXX      = gcc
AR       = ar
CXXFLAGS = -Wall -O6 -fomit-frame-pointer \
           -DPA_LITTLE_ENDIAN \
           -DSTDC_HEADERS=1 \
           -DHAVE_SYS_TYPES_H=1 \
           -DHAVE_SYS_STAT_H=1 \
           -DHAVE_STDLIB_H=1 \
           -DHAVE_STRING_H=1 \
           -DHAVE_MEMORY_H=1 \
           -DHAVE_STRINGS_H=1 \
           -DHAVE_INTTYPES_H=1 \
           -DHAVE_STDINT_H=1 \
           -DHAVE_UNISTD_H=1 \
           -DHAVE_DLFCN_H=1 \
           -DSIZEOF_SHORT=2 \
           -DSIZEOF_INT=4 \
           -DSIZEOF_LONG=4 \
           -DHAVE_NANOSLEEP=1 \
           -DPA_USE_COREAUDIO=1 \
           -DPIC
INCLUDE  = -I src/include \
           -I src/src/common \
           -I src/src/hostapi/coreaudio \
           -I src/src/os/mac_osx

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) cru $(TARGET) $(OBJECTS)
	cp src/include/*.h include

.c.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(<:.c=.o) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)
