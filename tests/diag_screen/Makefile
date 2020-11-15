#standalone ppsspp test version
COMPILER_ARTIFACTS = -I/usr/include/SDL2 -I../../ppsspp/ -I../../ppsspp/Common/

SOURCE_FILES =  main.cpp
SOURCE_FILES += ../../ppsspp/SDL/SDLMain.cpp
SOURCE_FILES += ../../ppsspp/UI/NativeApp.cpp
SOURCE_FILES += ../../ppsspp/SDL/SDLGLGraphicsContext.cpp
SOURCE_FILES += ../../ppsspp/SDL/SDLJoystick.cpp
SOURCE_FILES += ../../ppsspp/Common/System/Display.cpp
SOURCE_FILES += ../../ppsspp/Common/UI/Screen.cpp
SOURCE_FILES += ../../ppsspp/Common/TimeUtil.cpp
SOURCE_FILES += ../../ppsspp/Common/UI/Root.cpp
SOURCE_FILES += ../../ppsspp/Common/File/VFS/AssetReader.cpp
SOURCE_FILES += ../../ppsspp/Common/UI/View.cpp
SOURCE_FILES += ../../ppsspp/Common/UI/Context.cpp
SOURCE_FILES += ../../ppsspp/Common/Render/Text/draw_text.cpp
SOURCE_FILES += ../../ppsspp/Common/UI/UI.cpp
SOURCE_FILES += ../../ppsspp/Common/Render/DrawBuffer.cpp
SOURCE_FILES += ../../ppsspp/Common/Render/TextureAtlas.cpp
SOURCE_FILES += ../../ppsspp/Common/File/VFS/VFS.cpp
SOURCE_FILES += ../../ppsspp/Common/GPU/thin3d.cpp
SOURCE_FILES += ../../ppsspp/Common/Data/Text/WrapText.cpp
SOURCE_FILES += ../../ppsspp/UI/TextureUtil.cpp
SOURCE_FILES += ../../ppsspp/Common/Data/Color/RGBAUtil.cpp
SOURCE_FILES += ../../ppsspp/Common/ColorConv.cpp
SOURCE_FILES += ../../ppsspp/Common/Data/Encoding/Utf8.cpp
SOURCE_FILES += ../../ppsspp/Common/File/FileUtil.cpp
SOURCE_FILES += ../../ppsspp/Common/File/DirListing.cpp
SOURCE_FILES += ../../ppsspp/Common/Log.cpp
SOURCE_FILES += ../../ppsspp/Common/SysError.cpp 
SOURCE_FILES += ../../ppsspp/Common/Input/InputState.cpp
SOURCE_FILES += ../../ppsspp/Common/Data/Format/PNGLoad.cpp
SOURCE_FILES += ../../ppsspp/Common/Data/Format/ZIMLoad.cpp
SOURCE_FILES += ../../ppsspp/UI/MiscScreens.cpp
SOURCE_FILES += ../../ppsspp/Common/UI/UIScreen.cpp
SOURCE_FILES += ../../ppsspp/Common/Data/Text/I18n.cpp
SOURCE_FILES += ../../ppsspp/Common/Data/Format/IniFile.cpp
SOURCE_FILES += ../../ppsspp/Common/StringUtils.cpp
SOURCE_FILES += ../../ppsspp/Common/Buffer.cpp
SOURCE_FILES += ../../ppsspp/Common/Math/curves.cpp
SOURCE_FILES += ../../ppsspp/Common/UI/ViewGroup.cpp
SOURCE_FILES += ../../ppsspp/Common/Data/Text/Parsers.cpp
SOURCE_FILES += ../../ppsspp/Common/UI/Tween.cpp
SOURCE_FILES += ../../ppsspp/Common/File/FileDescriptor.cpp
SOURCE_FILES += ../../ppsspp/Common/GPU/OpenGL/GLRenderManager.cpp
SOURCE_FILES += ../../ppsspp/Common/GPU/OpenGL/GLQueueRunner.cpp
SOURCE_FILES += ../../ppsspp/Common/GPU/OpenGL/GLFeatures.cpp
SOURCE_FILES += ../../ppsspp/Common/GPU/OpenGL/thin3d_gl.cpp
SOURCE_FILES += ../../ppsspp/Common/MemoryUtil.cpp
SOURCE_FILES += ../../ppsspp/UI/OnScreenDisplay.cpp
SOURCE_FILES += ../../ppsspp/Common/Data/Convert/SmallDataConvert.cpp
SOURCE_FILES += ../../ppsspp/Common/GPU/OpenGL/DataFormatGL.cpp
SOURCE_FILES += ../../ppsspp/Common/Math/lin/matrix4x4.cpp
SOURCE_FILES += ../../ppsspp/Common/GPU/OpenGL/GLDebugLog.cpp
SOURCE_FILES += ../../ppsspp/Common/Math/fast/fast_matrix_sse.c
SOURCE_FILES += ../../ppsspp/UI/PauseScreenPCSX2.cpp

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
