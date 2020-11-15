#standalone ppsspp test version
COMPILER_ARTIFACTS = -I/usr/include/SDL2 -I../../screenManager/ -I../../screenManager/Common/

SOURCE_FILES =  main.cpp
SOURCE_FILES += ../../screenManager/SDL/SDLMain.cpp
SOURCE_FILES += ../../screenManager/UI/NativeApp.cpp
SOURCE_FILES += ../../screenManager/SDL/SDLGLGraphicsContext.cpp
SOURCE_FILES += ../../screenManager/SDL/SDLJoystick.cpp
SOURCE_FILES += ../../screenManager/Common/System/Display.cpp
SOURCE_FILES += ../../screenManager/Common/UI/Screen.cpp
SOURCE_FILES += ../../screenManager/Common/TimeUtil.cpp
SOURCE_FILES += ../../screenManager/Common/UI/Root.cpp
SOURCE_FILES += ../../screenManager/Common/File/VFS/AssetReader.cpp
SOURCE_FILES += ../../screenManager/Common/UI/View.cpp
SOURCE_FILES += ../../screenManager/Common/UI/Context.cpp
SOURCE_FILES += ../../screenManager/Common/Render/Text/draw_text.cpp
SOURCE_FILES += ../../screenManager/Common/UI/UI.cpp
SOURCE_FILES += ../../screenManager/Common/Render/DrawBuffer.cpp
SOURCE_FILES += ../../screenManager/Common/Render/TextureAtlas.cpp
SOURCE_FILES += ../../screenManager/Common/File/VFS/VFS.cpp
SOURCE_FILES += ../../screenManager/Common/GPU/thin3d.cpp
SOURCE_FILES += ../../screenManager/Common/Data/Text/WrapText.cpp
SOURCE_FILES += ../../screenManager/UI/TextureUtil.cpp
SOURCE_FILES += ../../screenManager/Common/Data/Color/RGBAUtil.cpp
SOURCE_FILES += ../../screenManager/Common/ColorConv.cpp
SOURCE_FILES += ../../screenManager/Common/Data/Encoding/Utf8.cpp
SOURCE_FILES += ../../screenManager/Common/File/FileUtil.cpp
SOURCE_FILES += ../../screenManager/Common/File/DirListing.cpp
SOURCE_FILES += ../../screenManager/Common/Log.cpp
SOURCE_FILES += ../../screenManager/Common/SysError.cpp 
SOURCE_FILES += ../../screenManager/Common/Input/InputState.cpp
SOURCE_FILES += ../../screenManager/Common/Data/Format/PNGLoad.cpp
SOURCE_FILES += ../../screenManager/Common/Data/Format/ZIMLoad.cpp
SOURCE_FILES += ../../screenManager/UI/MiscScreens.cpp
SOURCE_FILES += ../../screenManager/Common/UI/UIScreen.cpp
SOURCE_FILES += ../../screenManager/Common/Data/Text/I18n.cpp
SOURCE_FILES += ../../screenManager/Common/Data/Format/IniFile.cpp
SOURCE_FILES += ../../screenManager/Common/StringUtils.cpp
SOURCE_FILES += ../../screenManager/Common/Buffer.cpp
SOURCE_FILES += ../../screenManager/Common/Math/curves.cpp
SOURCE_FILES += ../../screenManager/Common/UI/ViewGroup.cpp
SOURCE_FILES += ../../screenManager/Common/Data/Text/Parsers.cpp
SOURCE_FILES += ../../screenManager/Common/UI/Tween.cpp
SOURCE_FILES += ../../screenManager/Common/File/FileDescriptor.cpp
SOURCE_FILES += ../../screenManager/Common/GPU/OpenGL/GLRenderManager.cpp
SOURCE_FILES += ../../screenManager/Common/GPU/OpenGL/GLQueueRunner.cpp
SOURCE_FILES += ../../screenManager/Common/GPU/OpenGL/GLFeatures.cpp
SOURCE_FILES += ../../screenManager/Common/GPU/OpenGL/thin3d_gl.cpp
SOURCE_FILES += ../../screenManager/Common/MemoryUtil.cpp
SOURCE_FILES += ../../screenManager/UI/OnScreenDisplay.cpp
SOURCE_FILES += ../../screenManager/Common/Data/Convert/SmallDataConvert.cpp
SOURCE_FILES += ../../screenManager/Common/GPU/OpenGL/DataFormatGL.cpp
SOURCE_FILES += ../../screenManager/Common/Math/lin/matrix4x4.cpp
SOURCE_FILES += ../../screenManager/Common/GPU/OpenGL/GLDebugLog.cpp
SOURCE_FILES += ../../screenManager/Common/Math/fast/fast_matrix_sse.c
SOURCE_FILES += ../../screenManager/UI/PauseScreenPCSX2.cpp

LINKER_OBJECTS_DYN 	= -lSDL2
LINKER_OBJECTS_DYN 	+= -lGLEW
LINKER_OBJECTS_DYN 	+= -lpthread
LINKER_OBJECTS_DYN 	+= -lz
LINKER_OBJECTS_DYN  += -ldl
LINKER_OBJECTS_DYN 	+= -lGL
LINKER_OBJECTS_DYN 	+= -lrt
LINKER_OBJECTS_DYN 	+= -lpng
LINKER_OBJECTS_DYN 	+= -lGLU

all: SETTINGS_DIAG

SETTINGS_DIAG: main.cpp
	$(info   *************** tests make settings dialog ***************)
	g++ $(COMPILER_ARTIFACTS) -o DIAG_SCREEN $(SOURCE_FILES) $(LINKER_OBJECTS_DYN)

clean:
	$(info   *************** tests settings dialog clean ***************)
	rm -rf *.o DIAG_SCREEN *.ini PSP
