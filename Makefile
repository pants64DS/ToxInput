SOURCE  := source
INCLUDE := include
BUILD   := build
BIN     := bin
EXE     := ToxInput.exe
TARGET  := $(BIN)/$(EXE)

SRC_FILES   := $(wildcard $(SOURCE)/*.cpp)
OBJ_FILES   := $(patsubst $(SOURCE)/%.cpp,$(BUILD)/%.o,$(SRC_FILES))

CXX         := C:/winlibs-x86_64-posix-seh-gcc-10.1.0-llvm-10.0.0-mingw-w64-7.0.0-r3/mingw64/bin/g++.exe

CPPFLAGS    := -I$(INCLUDE) -MMD -MP
CXXFLAGS    := -std=c++20 -Wall -Wextra -Wpedantic -Wno-switch -Wno-unused-parameter -O3

LDFLAGS     := -lXInput9_1_0 \
               $(BIN)/sfml-graphics-2.dll \
               $(BIN)/sfml-window-2.dll \
               $(BIN)/sfml-system-2.dll

$(TARGET): $(OBJ_FILES)
	$(CXX) -o $@ $^ icon/res.res $(LDFLAGS)

$(BUILD)/%.o: $(SOURCE)/%.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

-include $(OBJ_FILES:.o=.d)