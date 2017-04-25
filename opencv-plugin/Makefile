CXXFLAGS=-shared -fPIC -std=c++11
LDLIBS=-lopencv_core -lopencv_highgui -lopencv_calib3d
LDFLAGS=-L.

SRC=processor.cpp
OBJ=$(SRC:%.cpp=%.o)
CNTRLSRC=robot_controler.cpp
CNTRLOBJ=$(CNTRLSRC:%.cpp=%.o)
UNITYSRC=unity_controler.cpp
UNITYOBJ=$(UNITYSRC:%.cpp=%.o)

PLUGINSFOLDER=../Assets/Plugins

TARGET=lib$(SRC:%.cpp=%.so)
TARGETCNTRL=$(CNTRLSRC:%.cpp=%.so)
TARGETUNITY=$(UNITYSRC:%.cpp=%.so)

TARGETS = $(TARGET) $(TARGETCNTRL) $(TARGETUNITY) automate

all: | $(TARGETS)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ $<

automate: $(TARGET)
	make -C auto/ install

$(TARGETCNTRL): $(CNTRLOBJ) $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ $^

$(TARGETUNITY): $(UNITYOBJ) $(CNTRLOBJ) $(OBJ) | $(PLUGINSFOLDER)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) -lGL -o $@ $^
	cp $@ $(PLUGINSFOLDER)

$(PLUGINSFOLDER):
	mkdir -p $@


.PHONY: clean
clean:
	rm -f $(TARGETS) *.o
	make -C auto/ clean
	rm -rf $(PLUGINSFOLDER)
