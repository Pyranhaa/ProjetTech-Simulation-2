CC=g++

CXXFLAGS=-fpermissive -std=c++11 -I.. -Iinclude -g
LDFLAGS=-L/usr/local/opencv-2.4.13/lib -L.. -L. -Wl,-rpath=.
LDLIBS=-lopencv_core -lopencv_highgui -lprocessor

PARSER=src/cmdline.cpp
SRC=$(filter-out $(PARSER), $(wildcard src/*.cpp)) $(PARSER) #Si le parser est déjà généré, on l'ignore et on réajoute le fichier derrière
OBJ=$(SRC:src/%.cpp=%.o)

TARGET=automate

.PRECIOUS: %.o

all: $(TARGET)

$(TARGET): $(OBJ)
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
