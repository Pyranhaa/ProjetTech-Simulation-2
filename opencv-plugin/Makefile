CXXFLAGS=-shared -fPIC -std=c++11
LDLIBS=-lopencv_core -lopencv_highgui -lopencv_calib3d
LDFLAGS=-L.

SRC=processor.cpp
CNTRLSRC=robot_controler.cpp
CNTRLOBJ=$(CNTRLSRC:%.cpp=%.o)
UNITYSRC=unity_controler.cpp
UNITYOBJ=$(UNITYSRC:%.cpp=%.o)

TARGET=lib$(SRC:%.cpp=%.so)
TARGETCNTRL=$(CNTRLSRC:%.cpp=%)
TARGETUNITY=$(UNITYSRC:%.cpp=%.so)

TARGETS = $(TARGET) $(TARGETCNTRL) $(TARGETUNITY) automate

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ $<

automate: $(TARGET)
	make -C auto/ install

$(TARGETCNTRL): $(CNTRLOBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -lprocessor -o $@ $^

$(TARGETUNITY): $(UNITYOBJ) $(CNTRLOBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -lGL -o $@ $^

.PHONY: clean
clean:
	rm -f $(TARGETS) *.o
	make -C auto/ clean 
