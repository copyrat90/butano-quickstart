#---------------------------------------------------------------------------------------------------------------------
# TARGET is the name of the output.
# BUILD is the directory where object files & intermediate files will be placed.
# LIBBUTANO is the main directory of butano library (https://github.com/GValiente/butano).
# BUILDLDTK is the directory where LDtk converted files will be placed.
# LIBBUTANOLDTK is the root directory of butano-ldtk library (https://github.com/copyrat90/butano-ldtk).
# LDTKPROJECT is the path to the LDtk project file.
# PYTHON is the path to the python interpreter.
# SOURCES is a list of directories containing source code.
# INCLUDES is a list of directories containing extra header files.
# DATA is a list of directories containing binary data files with *.bin extension.
# FONTS is a list of directories containing font files.
# TEXTS is a list of directories or filenames containing text files.
# GRAPHICS is a list of files and directories containing files to be processed by grit.
# AUDIO is a list of files and directories containing files to be processed by the audio backend.
# AUDIOBACKEND specifies the backend used for audio playback. Supported backends: maxmod, aas, null.
# AUDIOTOOL is the path to the tool used process the audio files.
# DMGAUDIO is a list of files and directories containing files to be processed by the DMG audio backend.
# DMGAUDIOBACKEND specifies the backend used for DMG audio playback. Supported backends: default, null.
# ROMTITLE is a uppercase ASCII, max 12 characters text string containing the output ROM title.
# ROMCODE is a uppercase ASCII, max 4 characters text string containing the output ROM code.
# USERFLAGS is a list of additional compiler flags:
#     Pass -flto to enable link-time optimization.
#     Pass -O0 or -Og to try to make debugging work.
# USERCXXFLAGS is a list of additional compiler flags for C++ code only.
# USERASFLAGS is a list of additional assembler flags.
# USERLDFLAGS is a list of additional linker flags:
#     Pass -flto=<number_of_cpu_cores> to enable parallel link-time optimization.
# USERLIBDIRS is a list of additional directories containing libraries.
#     Each libraries directory must contains include and lib subdirectories.
# USERLIBS is a list of additional libraries to link with the project.
# DEFAULTLIBS links standard system libraries when it is not empty.
# STACKTRACE enables stack trace logging when it is not empty.
# USERBUILD is a list of additional directories to remove when cleaning the project.
# EXTTOOL is an optional command executed before processing audio, graphics and code files.
#
# All directories are specified relative to the project directory where the makefile is found.
#---------------------------------------------------------------------------------------------------------------------
TARGET      	:=  $(notdir $(CURDIR))
BUILD       	:=  build
LIBBUTANO   	:=  libs/butano/butano
BUILDLDTK   	:=  build_ldtk
LIBBUTANOLDTK	:=  libs/butano-ldtk
LDTKPROJECT 	:=  levels/project.ldtk
LIBISOBUTANO	:=  libs/iso-butano
BUILDFONTS  	:=  build_fonts
LIBGBAKORFONTS	:=  libs/gba-kor-fonts
BUILDMISC   	:=  build_misc
LICENSES    	:=  licenses
PYTHON      	:=  python
SOURCES     	:=  src src/sys src/scn $(LIBBUTANOLDTK)/src $(BUILDLDTK)/src $(LIBISOBUTANO)/src
INCLUDES    	:=  include $(LIBBUTANOLDTK)/include $(BUILDLDTK)/include $(LIBISOBUTANO)/include $(BUILDFONTS) $(BUILDMISC)/include
DATA        	:=  
FONTS       	:=  $(LIBGBAKORFONTS)/fonts/galmuri7 $(LIBGBAKORFONTS)/fonts/galmuri9 $(LIBGBAKORFONTS)/fonts/galmuri11 $(LIBGBAKORFONTS)/fonts/galmuri11_bold $(LIBGBAKORFONTS)/fonts/galmuri11_condensed
TEXTS       	:=  $(BUILDLDTK)/src $(BUILDLDTK)/include $(LICENSES)
GRAPHICS    	:=  graphics graphics/bg graphics/spr graphics/pal graphics/tile $(BUILDLDTK)/graphics $(BUILDFONTS)/fonts
AUDIO       	:=  audio
AUDIOBACKEND	:=  maxmod
AUDIOTOOL   	:=  
DMGAUDIO    	:=  dmg_audio
DMGAUDIOBACKEND	:=  default
ROMTITLE    	:=  QUICKSTART
ROMCODE     	:=  2BQE
USERFLAGS   	:=  
USERCXXFLAGS	:=  
USERASFLAGS 	:=  
USERLDFLAGS 	:=  
USERLIBDIRS 	:=  
USERLIBS    	:=  
DEFAULTLIBS 	:=  
STACKTRACE  	:=  YES
USERBUILD   	:=  $(BUILDLDTK) $(BUILDFONTS) $(BUILDMISC)
EXTTOOL     	:=  @$(PYTHON) -B tools/main.py --ldtk-project=$(LDTKPROJECT) --ldtk-build=$(BUILDLDTK) --fonts="$(FONTS)" --texts="$(TEXTS)" --fonts-build=$(BUILDFONTS) --licenses=$(LICENSES) --misc-build=$(BUILDMISC)

BQ_DEVBUILD 	:=  
ifneq ($(strip $(BQ_DEVBUILD)),)
	SOURCES 	+=  src/dev
	USERFLAGS	+=  -DBQ_DEVBUILD=true -DIBN_CFG_STATS_ENABLED=true
else
	USERFLAGS	+=  -DBQ_DEVBUILD=false -DIBN_CFG_STATS_ENABLED=false
endif

#---------------------------------------------------------------------------------------------------------------------
# Export absolute butano path:
#---------------------------------------------------------------------------------------------------------------------
ifndef LIBBUTANOABS
	export LIBBUTANOABS	:=	$(realpath $(LIBBUTANO))
endif

#---------------------------------------------------------------------------------------------------------------------
# Include main makefile:
#---------------------------------------------------------------------------------------------------------------------
include $(LIBBUTANOABS)/butano.mak
