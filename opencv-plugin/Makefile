CXXFLAGS=-shared -fPIC -std=c++11
LDLIBS=-lopencv_core -lopencv_highgui -lopencv_calib3d -lopencv_imgproc

PROCSRC=processor.cpp
PROCOBJ=$(PROCSRC:%.cpp=%.o)
CNTRLSRC=robot_controler.cpp
CNTRLOBJ=$(CNTRLSRC:%.cpp=%.o)
UNITYSRC=unity_controler.cpp
UNITYOBJ=$(UNITYSRC:%.cpp=%.o)

PLUGINSFOLDER=../Assets/Plugins

TARGETCNTRL=$(CNTRLSRC:%.cpp=%.so)
TARGETUNITY=$(UNITYSRC:%.cpp=%.so)

TARGETS = $(TARGETCNTRL) $(TARGETUNITY) automate

all: | $(TARGETS)

automate: $(PROCOBJ)
	make -C auto/ install

$(TARGETCNTRL): $(CNTRLOBJ) $(PROCOBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ $^

$(TARGETUNITY): $(UNITYOBJ) $(CNTRLOBJ) $(PROCOBJ) | $(PLUGINSFOLDER)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) -lGL -o $@ $^
	cp $@ $(PLUGINSFOLDER)

$(PLUGINSFOLDER):
	mkdir -p $@


.PHONY: clean
clean:
	rm -f $(TARGETS) *.o
	make -C auto/ clean
	rm -rf $(PLUGINSFOLDER)
