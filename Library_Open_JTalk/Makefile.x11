
TARGET   = lib/Open_JTalk.a

SOURCES  = src/jpcommon/jpcommon.c \
           src/jpcommon/jpcommon_label.c \
           src/jpcommon/jpcommon_node.c \
           src/mecab2njd/mecab2njd.c \
           src/njd/njd.c \
           src/njd/njd_node.c \
           src/njd2jpcommon/njd2jpcommon.c \
           src/njd_set_accent_phrase/njd_set_accent_phrase.c \
           src/njd_set_accent_type/njd_set_accent_type.c \
           src/njd_set_digit/njd_set_digit.c \
           src/njd_set_long_vowel/njd_set_long_vowel.c \
           src/njd_set_pronunciation/njd_set_pronunciation.c \
           src/njd_set_unvoiced_vowel/njd_set_unvoiced_vowel.c \
           src/text2mecab/text2mecab.c

CXXSOURCES = src/mecab/src/char_property.cpp \
           src/mecab/src/connector.cpp \
           src/mecab/src/context_id.cpp \
           src/mecab/src/dictionary.cpp \
           src/mecab/src/dictionary_compiler.cpp \
           src/mecab/src/dictionary_generator.cpp \
           src/mecab/src/dictionary_rewriter.cpp \
           src/mecab/src/eval.cpp \
           src/mecab/src/feature_index.cpp \
           src/mecab/src/iconv_utils.cpp \
           src/mecab/src/lbfgs.cpp \
           src/mecab/src/learner.cpp \
           src/mecab/src/learner_tagger.cpp \
           src/mecab/src/libmecab.cpp \
           src/mecab/src/mecab.cpp \
           src/mecab/src/nbest_generator.cpp \
           src/mecab/src/param.cpp \
           src/mecab/src/string_buffer.cpp \
           src/mecab/src/tagger.cpp \
           src/mecab/src/tokenizer.cpp \
           src/mecab/src/utils.cpp \
           src/mecab/src/viterbi.cpp \
           src/mecab/src/writer.cpp

OBJECTS  = $(SOURCES:.c=.o) $(CXXSOURCES:.cpp=.o)

CXX      = gcc
AR       = ar
CXXFLAGS = -Wall -O6 -fomit-frame-pointer \
           -finput-charset=cp932 \
           -fexec-charset=cp932 \
           -D DIC_VERSION=102 \
           -D MECAB_DEFAULT_RC="\"dummy\"" \
           -D PACKAGE="\"open_jtalk\"" \
           -D VERSION="\"1.05\"" \
           -D MECAB_WITHOUT_SHARE_DIC \
           -D CHARSET_SHIFT_JIS \
           -D HAVE_CTYPE_H=1 \
           -D HAVE_DIRENT_H=1 \
           -D HAVE_FCNTL_H=1 \
           -D HAVE_GETENV=1 \
           -D HAVE_GETPAGESIZE=1 \
           -D HAVE_INTTYPES_H=1 \
           -D HAVE_LIBM=1 \
           -D HAVE_MEMORY_H=1 \
           -D HAVE_MMAP=1 \
           -D HAVE_OPENDIR=1 \
           -D HAVE_SETJMP=1 \
           -D HAVE_SETJMP_H=1 \
           -D HAVE_SQRT=1 \
           -D HAVE_STDINT_H=1 \
           -D HAVE_STDLIB_H=1 \
           -D HAVE_STRINGS_H=1 \
           -D HAVE_STRING_H=1 \
           -D HAVE_STRSTR=1 \
           -D HAVE_SYS_MMAN_H=1 \
           -D HAVE_SYS_STAT_H=1 \
           -D HAVE_SYS_TIMES_H=1 \
           -D HAVE_SYS_TYPES_H=1 \
           -D HAVE_UNISTD_H=1 \
           -D PACKAGE_BUGREPORT="open-jtalk-users@lists.sourceforge.net" \
           -D PACKAGE_NAME="open_jtalk" \
           -D PACKAGE_STRING="open_jtalk 1.04" \
           -D PACKAGE_TARNAME="open_jtalk" \
           -D PACKAGE_VERSION="1.04" \
           -D SIZEOF_CHAR=1 \
           -D SIZEOF_INT=4 \
           -D SIZEOF_LONG=4 \
           -D SIZEOF_LONG_LONG=8 \
           -D SIZEOF_SHORT=2 \
           -D SIZEOF_SIZE_T=4 \
           -D STDC_HEADERS=1 \
           -D MECAB_DEFAULT_CHARSET="\"SHIFT-JIS\""
INCLUDE  = -I src/jpcommon \
           -I src/mecab/src \
           -I src/mecab2njd \
           -I src/njd \
           -I src/njd2jpcommon \
           -I src/njd_set_accent_phrase \
           -I src/njd_set_accent_type \
           -I src/njd_set_digit \
           -I src/njd_set_long_vowel \
           -I src/njd_set_pronunciation \
           -I src/njd_set_unvoiced_vowel \
           -I src/text2mecab

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) cru $(TARGET) $(OBJECTS)
	cp src/jpcommon/jpcommon.h include
	cp src/mecab/src/mecab.h include
	cp src/mecab2njd/mecab2njd.h include
	cp src/njd/njd.h include
	cp src/njd_set_accent_phrase/njd_set_accent_phrase.h include
	cp src/njd_set_accent_type/njd_set_accent_type.h include
	cp src/njd_set_digit/njd_set_digit.h include
	cp src/njd_set_long_vowel/njd_set_long_vowel.h include
	cp src/njd_set_pronunciation/njd_set_pronunciation.h include
	cp src/njd_set_unvoiced_vowel/njd_set_unvoiced_vowel.h include
	cp src/text2mecab/text2mecab.h include

.c.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(<:.c=.o) -c $<

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(<:.cpp=.o) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)
