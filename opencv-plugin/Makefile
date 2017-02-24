CXXFLAGS=-shared -fPIC -std=c++11

SRC=libProcessor.cpp

TARGET=libProcessor.so

all: unity

unity: $(SRC)
	$(CXX) $(CXXFLAGS) -D UNITY -o $(TARGET) $<


.PHONY: strip
strip: $(SRC) #Même build mais en unset UNITY
	$(CXX) $(CXXFLAGS) -o $(TARGET) $<	

automate: strip
	make -C auto/ install

.PHONY: clean
clean:
	rm -f $(TARGET) automate