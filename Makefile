CC = g++
CFLAGS = -ggdb -O3 `pkg-config --cflags opencv`
LDFLAGS = `pkg-config --libs opencv`

OUT = SideFacingCamera TakePicture
SOURCES = SideFacingCamera.cpp TakePicture.cpp

all: $(OUT)

SideFacingCamera: SideFacingCamera.cpp
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

TakePicture: TakePicture.cpp
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)