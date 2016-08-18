# Project: SCGL

ROOT_DIR := $(CURDIR)

CPP      = g++
CC       = gcc
OBJ      = out/SCGL.o
LINKOBJ  = out/SCGL.o
LIBS     = -L"./lib" --disable-shared -lcurl -lcurldll -luser32 -lgdi32 -lkernel32 -lwsock32 -lpthreadGC -m32 -Wl,--oformat,pei-i386 -lgcc -lstdc++
INCS     =
CXXINCS  =
BIN      = out/SCGL.exe
CXXFLAGS = $(CXXINCS) -static-libgcc -static-libstdc++ -Wl,--oformat,pei-i386 -lm -std=gnu++0x -O2 -w -Wall -m32
CFLAGS   = $(INCS) -static-libgcc -Wl,--oformat,pei-i386 -lm -std=gnu++0x -O2 -w -Wall -m32
RM       = rm -f -r -d
ASSETDIR = assets
OPTAG = @
GIT = git
START = ./out/SCGL.exe

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after build-assets

prepare:
	${OPTAG}echo "[INIT] Preparing directory layout..."
	${OPTAG}mkdir -p ./release/
	${OPTAG}mkdir -p ./out/
	${OPTAG}echo "[INIT] Init done."

build-assets: prepare
	${OPTAG}echo "[BUILDING] Copying assets to the output directory..."
	${OPTAG}cp ./$(ASSETDIR)/* ./out/
	${OPTAG}echo "[BUILDING] Copying version info to the output directory..."
	${OPTAG}cp ./version/version.info ./out/version_.info
	${OPTAG}cp ./version/version.info ./out/version.info
	${OPTAG}echo "[BUILDING] Copying runtime/utils to the output directory..."
	${OPTAG}cp ./runtime/* ./out/
	${OPTAG}cp ./utils/* ./out/
	${OPTAG}echo "[BUILDING] Removing temporary build data..."
	${OPTAG}${RM} utils/SCGL.zip
	${OPTAG}echo "[BUILDING] Compressing build..."
	${OPTAG}utils/fbzip.exe -a -r "SCGL.zip" "$(ROOT_DIR)/out"
	${OPTAG}echo "[BUILDING] Copying build to the final destination..."
	${OPTAG}cp ./utils/SCGL.zip ./release/
	${OPTAG}echo "[BUILDING] Removing temporary build data..."
	${OPTAG}${RM} utils/SCGL.zip
	${OPTAG}echo "[BUILDING] Finalizing build..."
	${OPTAG}cp ./version/version.info ./release/version.info
	${OPTAG}echo "[DONE] Build done."

clean: clean-custom
	${OPTAG}echo "[CLEANING] Cleaning build..."
	${OPTAG}${RM} $(OBJ) $(BIN)
	${OPTAG}${RM} ./out/**/*
	${OPTAG}${RM} ./release/**/*
	${OPTAG}echo "[DONE] Build clean."

$(BIN): $(OBJ)
	${OPTAG}echo "[BUILDING] Linking object files..."
	${OPTAG}echo "[INFO] $(CPP) $(LINKOBJ) -o $(BIN) $(LIBS) $(CXXFLAGS)"
	${OPTAG}$(CPP) $(LINKOBJ) -o $(BIN) $(LIBS) $(CXXFLAGS)

out/SCGL.o: SCGL.cpp
	${OPTAG}echo "[BUILDING] Compiling sources."
	${OPTAG}echo "[INFO] $(CPP) -c SCGL.cpp -o out/SCGL.o $(CXXFLAGS)"
	${OPTAG}$(CPP) -c SCGL.cpp -o out/SCGL.o $(CXXFLAGS)

commit:
	${GIT} add -A
	${GIT} commit
	${GIT} push

start:
	${START}
