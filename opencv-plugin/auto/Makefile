CC=g++

CXXFLAGS=-fpermissive -std=c++11 -I..
LDFLAGS=-L/usr/local/opencv-2.4.13/lib -L.. -L. -Wl,-rpath=.
LDLIBS=-lopencv_core -lopencv_highgui -lprocessor #-lGL

TARGET=automate


all: $(TARGET)

$(TARGET): cmdline.o main.o
	$(CXX) -o $@ $(LDFLAGS) $(LDLIBS) $^

install: $(TARGET)
	cp $(TARGET) ../

cmdline.c cmdline.h: options.ggo
	gengetopt < options.ggo

	
.PHONY: clean
clean:
	$(RM) -rf *.o $(TARGET) cmdline*
