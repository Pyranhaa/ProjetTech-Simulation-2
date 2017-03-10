CXXFLAGS=-shared -fPIC -std=c++11
#LDFLAGS=-L/usr/local/opencv-2.4.13/lib
LDLIBS=-lopencv_core -lopencv_highgui -lopencv_calib3d#-lGL


SRC=processor.cpp

TARGET=lib$(SRC:%.cpp=%.so)

all: unity

.PHONY: unity
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
	make -C auto/ clean 
