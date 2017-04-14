CXXFLAGS=-shared -fPIC -std=c++11
LDLIBS=-lopencv_core -lopencv_highgui -lopencv_calib3d#-lGL
LDFLAGS=-L.

SRC=processor.cpp
CNTRLSRC=robot_controler.cpp
CNTRLOBJ=$(CNTRLSRC:%.cpp=%.o)

TARGET=lib$(SRC:%.cpp=%.so)

all: strip

.PHONY: unity
unity: $(SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) -D UNITY -o $(TARGET) $<


.PHONY: strip
strip: $(SRC) #Même build mais en unset UNITY
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) -o $(TARGET) $<

automate: strip
	make -C auto/ install

controler: $(CNTRLOBJ) | strip
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) -lprocessor -o $@ $^
	
.PHONY: clean
clean:
	rm -f $(TARGET) automate
	make -C auto/ clean 
