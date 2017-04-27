CXXFLAGS=-fpermissive -std=c++11 -I.. -Iinclude
LDLIBS=-lopencv_core -lopencv_highgui -lopencv_calib3d -lopencv_imgproc

PARSER=src/cmdline.cpp
SRC=$(filter-out $(PARSER), $(wildcard src/*.cpp)) $(PARSER) #Si le parser est déjà généré, on l'ignore et on réajoute le fichier derrière
OBJ=$(SRC:src/%.cpp=%.o)

TARGET=automate

.PRECIOUS: %.o

all: $(TARGET)

$(TARGET): $(OBJ) ../processor.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ $^

$(OBJ): %.o: src/%.cpp | $(PARSER)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

install: $(TARGET)
	cp $(TARGET) ../

$(PARSER): options.ggo
	gengetopt --header-output-dir="include" --src-output-dir="src" < options.ggo

	
.PHONY: clean
clean:
	$(RM) -rf *.o $(TARGET) $(PARSER) $(PARSER:src/%.cpp=include/%.h)
