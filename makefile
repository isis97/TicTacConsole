# Project: SCGL

ROOT_DIR := $(CURDIR)

CPP      = g++
CC       = gcc
WINDRES  = windres.exe
OBJ      = out/SCGL.o
LINKOBJ  = out/SCGL.o
LIBS     = -L"./lib" --disable-shared -lcurl -lcurldll -luser32 -lgdi32 -lkernel32 -lwsock32 -lpthreadGC -m32 -Wl,--oformat,pei-i386 -lgcc -lstdc++
INCS     = -I"C:/MinGW/include"
CXXINCS  = -I"C:/MinGW/include"
BIN      = out/SCGL.exe
CXXFLAGS = $(CXXINCS) -static-libgcc -static-libstdc++ -Wl,--oformat,pei-i386 -lm -std=gnu++0x -O2 -w -Wall -m32 
CFLAGS   = $(INCS) -static-libgcc -Wl,--oformat,pei-i386 -lm -std=gnu++0x -O2 -w -Wall -m32
RM       = rm -f
ASSETDIR = assets

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after build-assets

build-assets:
	cp ./$(ASSETDIR)/* ./out/
	cp ./version/version.info ./out/version_.info
	cp ./version/version.info ./out/version.info
	cp ./runtime/* ./out/
	cp ./utils/* ./out/
	${RM} utils/SCGL.zip
	utils/fbzip.exe -a -r "SCGL.zip" "$(ROOT_DIR)/out"
	cp ./utils/SCGL.zip ./release/
	${RM} utils/SCGL.zip
	cp ./version/version.info ./release/version.info
	echo ""

clean: clean-custom
	${RM} $(OBJ) $(BIN)
	${RM} ./out/**/*
	${RM} ./release/**/*

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN) $(LIBS) $(CXXFLAGS)

out/SCGL.o: SCGL.cpp
	$(CPP) -c SCGL.cpp -o out/SCGL.o $(CXXFLAGS)

