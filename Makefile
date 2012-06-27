
all:
	@echo "--------------------------------------------------------------------------------------------"
	@echo "  make mingw            to compile MMDAgent for MinGW on Windows"
	@echo "  make mingw-clean      to remove any files generated for MinGW on Windows"
	@echo "--------------------------------------------------------------------------------------------"
	@echo "  make x11-euc-jp       to compile MMDAgent for X11 on Unix-like systems (EUC-JP file system)"
	@echo "  make x11-euc-jp-clean to remove any files generated for X11 on Unix-like systems (EUC-JP file system)"
	@echo "--------------------------------------------------------------------------------------------"
	@echo "  make x11-utf-8        to compile MMDAgent for X11 on Unix-like systems (UTF-8 file system)"
	@echo "  make x11-utf-8-clean  to remove any files generated for X11 on Unix-like systems (UTF-8 file system)"
	@echo "--------------------------------------------------------------------------------------------"
	@echo "  make carbon           to compile MMDAgent for Carbon on Mac OS X"
	@echo "  make carbon-clean     to remove any files generated for Carbon on Mac OS X"
	@echo "--------------------------------------------------------------------------------------------"

mingw:
	cd Library_Bullet_Physics  && make
	cd Library_GLFW            && make -f Makefile.mingw
	cd Library_GLee            && make
	cd Library_JPEG            && make
	cd Library_libpng          && make
	cd Library_MMDAgent        && make
	cd Library_MMDFiles        && make
	cd Library_zlib            && make
	cd Library_PortAudio       && make -f Makefile.mingw
	cd Library_hts_engine_API  && make
	cd Library_Flite           && make
	cd Library_Julius          && make -f Makefile.mingw
	cd main                    && make -f Makefile.mingw
	cd Plugin_VIManager        && make -f Makefile.mingw
	cd Plugin_Variables        && make -f Makefile.mingw
	cd Plugin_Audio            && make -f Makefile.mingw
	cd Plugin_LookAt           && make -f Makefile.mingw
	cd Plugin_WindowController && make -f Makefile.mingw
	cd Plugin_Flite            && make
	cd Plugin_Julius           && make -f Makefile.mingw

mingw-clean:
	cd Library_Bullet_Physics  && make                   clean
	cd Library_GLFW            && make -f Makefile.mingw clean
	cd Library_GLee            && make                   clean
	cd Library_JPEG            && make                   clean
	cd Library_libpng          && make                   clean
	cd Library_MMDAgent        && make                   clean
	cd Library_MMDFiles        && make                   clean
	cd Library_zlib            && make                   clean
	cd Library_PortAudio       && make -f Makefile.mingw clean
	cd Library_hts_engine_API  && make                   clean
	cd Library_Flite           && make                   clean
	cd Library_Julius          && make -f Makefile.mingw clean
	cd main                    && make -f Makefile.mingw clean
	cd Plugin_VIManager        && make -f Makefile.mingw clean
	cd Plugin_Variables        && make -f Makefile.mingw clean
	cd Plugin_Audio            && make -f Makefile.mingw clean
	cd Plugin_LookAt           && make -f Makefile.mingw clean
	cd Plugin_WindowController && make -f Makefile.mingw clean
	cd Plugin_Flite            && make                   clean
	cd Plugin_Julius           && make -f Makefile.mingw clean

x11-euc-jp:
	cd Library_Bullet_Physics && make
	cd Library_GLFW           && make -f Makefile.x11
	cd Library_GLee           && make
	cd Library_JPEG           && make
	cd Library_libpng         && make
	cd Library_MMDAgent       && make
	cd Library_MMDFiles       && make -f Makefile.x11-euc-jp
	cd Library_zlib           && make
	cd Library_PortAudio      && make -f Makefile.x11
	cd Library_hts_engine_API && make
	cd Library_Flite          && make
	cd Library_Julius         && make -f Makefile.x11
	cd main                   && make -f Makefile.x11-euc-jp
	cd Plugin_VIManager       && make -f Makefile.x11
	cd Plugin_Variables       && make -f Makefile.x11
	cd Plugin_LookAt          && make -f Makefile.x11
	cd Plugin_Flite           && make
	cd Plugin_Julius          && make -f Makefile.x11

x11-euc-jp-clean:
	cd Library_Bullet_Physics && make                        clean
	cd Library_GLFW           && make -f Makefile.x11        clean
	cd Library_GLee           && make                        clean
	cd Library_JPEG           && make                        clean
	cd Library_libpng         && make                        clean
	cd Library_MMDAgent       && make                        clean
	cd Library_MMDFiles       && make -f Makefile.x11-euc-jp clean
	cd Library_zlib           && make                        clean
	cd Library_PortAudio      && make -f Makefile.x11        clean
	cd Library_hts_engine_API && make                        clean
	cd Library_Flite          && make                        clean
	cd Library_Julius         && make -f Makefile.x11        clean
	cd main                   && make -f Makefile.x11-euc-jp clean
	cd Plugin_VIManager       && make -f Makefile.x11        clean
	cd Plugin_Variables       && make -f Makefile.x11        clean
	cd Plugin_LookAt          && make -f Makefile.x11        clean
	cd Plugin_Flite           && make                        clean
	cd Plugin_Julius          && make -f Makefile.x11        clean

x11-utf-8:
	cd Library_Bullet_Physics && make
	cd Library_GLFW           && make -f Makefile.x11
	cd Library_GLee           && make
	cd Library_JPEG           && make
	cd Library_libpng         && make
	cd Library_MMDAgent       && make
	cd Library_MMDFiles       && make -f Makefile.x11-utf-8
	cd Library_zlib           && make
	cd Library_PortAudio      && make -f Makefile.x11
	cd Library_Flite          && make
	cd main                   && make -f Makefile.x11-utf-8
	cd Plugin_VIManager       && make -f Makefile.x11
	cd Plugin_Variables       && make -f Makefile.x11
	cd Plugin_LookAt          && make -f Makefile.x11
	cd Plugin_Flite           && make

x11-utf-8-clean:
	cd Library_Bullet_Physics && make                       clean
	cd Library_GLFW           && make -f Makefile.x11       clean
	cd Library_GLee           && make                       clean
	cd Library_JPEG           && make                       clean
	cd Library_libpng         && make                       clean
	cd Library_MMDAgent       && make                       clean
	cd Library_MMDFiles       && make -f Makefile.x11-utf-8 clean
	cd Library_zlib           && make                       clean
	cd Library_PortAudio      && make -f Makefile.x11       clean
	cd Library_Flite          && make                       clean
	cd main                   && make -f Makefile.x11-utf-8 clean
	cd Plugin_VIManager       && make -f Makefile.x11       clean
	cd Plugin_Variables       && make -f Makefile.x11       clean
	cd Plugin_LookAt          && make -f Makefile.x11       clean
	cd Plugin_Flite           && make -f Makefile           clean

carbon:
	cd Library_Bullet_Physics && make
	cd Library_GLFW           && make -f Makefile.carbon
	cd Library_GLee           && make
	cd Library_JPEG           && make
	cd Library_libpng         && make
	cd Library_MMDAgent       && make
	cd Library_MMDFiles       && make
	cd Library_zlib           && make
	cd Library_PortAudio      && make -f Makefile.carbon
	cd Library_hts_engine_API && make
	cd Library_Flite          && make
	cd Library_Julius         && make -f Makefile.carbon
	cd main                   && make -f Makefile.carbon
	cd Plugin_VIManager       && make -f Makefile.carbon
	cd Plugin_Variables       && make -f Makefile.carbon
	cd Plugin_Audio           && make -f Makefile.carbon
	cd Plugin_LookAt          && make -f Makefile.carbon
	cd Plugin_Flite           && make
	cd Plugin_Julius          && make -f Makefile.carbon

carbon-clean:
	cd Library_Bullet_Physics && make                    clean
	cd Library_GLFW           && make -f Makefile.carbon clean
	cd Library_GLee           && make                    clean
	cd Library_JPEG           && make                    clean
	cd Library_libpng         && make                    clean
	cd Library_MMDAgent       && make                    clean
	cd Library_MMDFiles       && make                    clean
	cd Library_zlib           && make                    clean
	cd Library_PortAudio      && make -f Makefile.carbon clean
	cd Library_hts_engine_API && make                    clean
	cd Library_Flite          && make                    clean
	cd Library_Julius         && make -f Makefile.carbon clean
	cd main                   && make -f Makefile.carbon clean
	cd Plugin_VIManager       && make -f Makefile.carbon clean
	cd Plugin_Variables       && make -f Makefile.carbon clean
	cd Plugin_Audio           && make -f Makefile.carbon clean
	cd Plugin_LookAt          && make -f Makefile.carbon clean
	cd Plugin_Flite           && make                    clean
	cd Plugin_Julius          && make -f Makefile.carbon clean
