CXXFLAGS=-shared -fPIC -std=c++11
LDFLAGS=-L/usr/local/opencv-2.4.13/lib
LDLIBS=-lopencv_core -lopencv_highgui #-lGL


SRC=libProcessor.cpp

TARGET=libProcessor.so

all: unity

unity: $(SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) -D UNITY -o $(TARGET) $<


.PHONY: strip
strip: $(SRC) #Même build mais en unset UNITY
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) -o $(TARGET) $<
automate: strip
	make -C auto/ install

.PHONY: clean
clean:
	rm -f $(TARGET) automate
	make -sC auto/ clean 