
TARGET   = ../Release/MMDAgent.app/Contents/MacOS/MMDAgent

SOURCES  = main.cpp

OBJECTS  = $(SOURCES:.cpp=.o)

LDADD    = ../Library_MMDAgent/lib/MMDAgent.a \
           ../Library_MMDFiles/lib/MMDFiles.a \
           ../Library_GLFW/lib/GLFW.a \
           ../Library_GLee/lib/GLee.a \
           ../Library_Bullet_Physics/lib/Bullet_Physics.a \
           ../Library_libpng/lib/libpng.a \
           ../Library_JPEG/lib/JPEG.a \
           ../Library_zlib/lib/zlib.a

CXX      = gcc
AR       = ar
CXXFLAGS = -Wall -O6 -fomit-frame-pointer \
           -m32 \
           -DMMDAGENT
INCLUDE  = -I ../Library_Bullet_Physics/include \
           -I ../Library_GLee/include \
           -I ../Library_GLFW/include \
           -I ../Library_MMDFiles/include \
           -I ../Library_MMDAgent/include

all: $(TARGET)

$(TARGET): $(OBJECTS) $(LDADD) ../Release/MMDAgent.app/Contents/Info.plist
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LDADD) -o $(TARGET) \
	-lstdc++ -framework AGL -framework Carbon -framework OpenGL

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(<:.cpp=.o) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)

../Release/MMDAgent.app/Contents/Info.plist:
	mkdir -p ../Release/MMDAgent.app/Contents/MacOS
	mkdir -p ../Release/MMDAgent.app/Contents/MacOS/AppData
	mkdir -p ../Release/MMDAgent.app/Contents/MacOS/AppData/Julius
	mkdir -p ../Release/MMDAgent.app/Contents/MacOS/AppData/Julius/lang_m
	mkdir -p ../Release/MMDAgent.app/Contents/MacOS/AppData/Julius/phone_m
	mkdir -p ../Release/MMDAgent.app/Contents/MacOS/AppData/Open_JTalk
	mkdir -p ../Release/MMDAgent.app/Contents/MacOS/Plugins
	mkdir -p ../Release/MMDAgent.app/Contents/Resources
	cp ../Release/AppData/Julius/jconf.txt ../Release/MMDAgent.app/Contents/MacOS/AppData/Julius/jconf.txt
	cp ../Release/AppData/Julius/lang_m/COPYRIGHT.txt ../Release/MMDAgent.app/Contents/MacOS/AppData/Julius/lang_m/COPYRIGHT.txt
	cp ../Release/AppData/Julius/lang_m/web.60k.8-8.bingramv5.gz ../Release/MMDAgent.app/Contents/MacOS/AppData/Julius/lang_m/web.60k.8-8.bingramv5.gz
	cp ../Release/AppData/Julius/lang_m/web.60k.htkdic ../Release/MMDAgent.app/Contents/MacOS/AppData/Julius/lang_m/web.60k.htkdic
	cp ../Release/AppData/Julius/phone_m/COPYRIGHT.txt ../Release/MMDAgent.app/Contents/MacOS/AppData/Julius/phone_m/COPYRIGHT.txt
	cp ../Release/AppData/Julius/phone_m/README.txt ../Release/MMDAgent.app/Contents/MacOS/AppData/Julius/phone_m/README.txt
	cp ../Release/AppData/Julius/phone_m/clustered.mmf.16mix.all.julius.binhmm ../Release/MMDAgent.app/Contents/MacOS/AppData/Julius/phone_m/clustered.mmf.16mix.all.julius.binhmm
	cp ../Release/AppData/Julius/phone_m/tri_tied.list.bin ../Release/MMDAgent.app/Contents/MacOS/AppData/Julius/phone_m/tri_tied.list.bin
	cp ../Release/AppData/Open_JTalk/COPYRIGHT.txt ../Release/MMDAgent.app/Contents/MacOS/AppData/Open_JTalk/COPYRIGHT.txt
	cp ../Release/AppData/Open_JTalk/char.bin ../Release/MMDAgent.app/Contents/MacOS/AppData/Open_JTalk/char.bin
	cp ../Release/AppData/Open_JTalk/left-id.def ../Release/MMDAgent.app/Contents/MacOS/AppData/Open_JTalk/left-id.def
	cp ../Release/AppData/Open_JTalk/matrix.bin ../Release/MMDAgent.app/Contents/MacOS/AppData/Open_JTalk/matrix.bin
	cp ../Release/AppData/Open_JTalk/pos-id.def ../Release/MMDAgent.app/Contents/MacOS/AppData/Open_JTalk/pos-id.def
	cp ../Release/AppData/Open_JTalk/rewrite.def ../Release/MMDAgent.app/Contents/MacOS/AppData/Open_JTalk/rewrite.def
	cp ../Release/AppData/Open_JTalk/right-id.def ../Release/MMDAgent.app/Contents/MacOS/AppData/Open_JTalk/right-id.def
	cp ../Release/AppData/Open_JTalk/sys.dic ../Release/MMDAgent.app/Contents/MacOS/AppData/Open_JTalk/sys.dic
	cp ../Release/AppData/Open_JTalk/unk.dic ../Release/MMDAgent.app/Contents/MacOS/AppData/Open_JTalk/unk.dic
	cp ../Release/AppData/lip.txt ../Release/MMDAgent.app/Contents/MacOS/AppData/lip.txt
	cp ../Release/AppData/toon0.bmp ../Release/MMDAgent.app/Contents/MacOS/AppData/toon0.bmp
	cp ../Release/AppData/toon01.bmp ../Release/MMDAgent.app/Contents/MacOS/AppData/toon01.bmp
	cp ../Release/AppData/toon02.bmp ../Release/MMDAgent.app/Contents/MacOS/AppData/toon02.bmp
	cp ../Release/AppData/toon03.bmp ../Release/MMDAgent.app/Contents/MacOS/AppData/toon03.bmp
	cp ../Release/AppData/toon04.bmp ../Release/MMDAgent.app/Contents/MacOS/AppData/toon04.bmp
	cp ../Release/AppData/toon05.bmp ../Release/MMDAgent.app/Contents/MacOS/AppData/toon05.bmp
	cp ../Release/AppData/toon06.bmp ../Release/MMDAgent.app/Contents/MacOS/AppData/toon06.bmp
	cp ../Release/AppData/toon07.bmp ../Release/MMDAgent.app/Contents/MacOS/AppData/toon07.bmp
	cp ../Release/AppData/toon08.bmp ../Release/MMDAgent.app/Contents/MacOS/AppData/toon08.bmp
	cp ../Release/AppData/toon09.bmp ../Release/MMDAgent.app/Contents/MacOS/AppData/toon09.bmp
	cp ../Release/AppData/toon10.bmp ../Release/MMDAgent.app/Contents/MacOS/AppData/toon10.bmp
	cp ../Release/COPYRIGHT.txt ../Release/MMDAgent.app/Contents/MacOS/COPYRIGHT.txt
	cp ../Release/MMDAgent.mdf ../Release/MMDAgent.app/Contents/MacOS/MMDAgent.mdf
	cp ../Release/NEWS.txt ../Release/MMDAgent.app/Contents/MacOS/NEWS.txt
	cp ../Release/README.txt ../Release/MMDAgent.app/Contents/MacOS/README.txt
	cp main.icns ../Release/MMDAgent.app/Contents/Resources/main.icns
	echo '<?xml version="1.0" encoding="UTF-8"?>' > ../Release/MMDAgent.app/Contents/Info.plist
	echo '<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">' >> ../Release/MMDAgent.app/Contents/Info.plist
	echo '<plist version="1.0">' >> ../Release/MMDAgent.app/Contents/Info.plist
	echo '<dict>' >> ../Release/MMDAgent.app/Contents/Info.plist
	echo '<key>CFBundleExecutable</key>' >> ../Release/MMDAgent.app/Contents/Info.plist
	echo '<string>MMDAgent</string>' >> ../Release/MMDAgent.app/Contents/Info.plist
	echo '<key>CFBundleIconFile</key>' >> ../Release/MMDAgent.app/Contents/Info.plist
	echo '<string>main</string>' >> ../Release/MMDAgent.app/Contents/Info.plist
	echo '<key>CFBundleName</key>' >> ../Release/MMDAgent.app/Contents/Info.plist
	echo '<string>MMDAgent</string>' >> ../Release/MMDAgent.app/Contents/Info.plist
	echo '</dict>' >> ../Release/MMDAgent.app/Contents/Info.plist
	echo '</plist>' >> ../Release/MMDAgent.app/Contents/Info.plist
	touch ../Release/MMDAgent.app
