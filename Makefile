SOURCES = main.cpp

OBJECTS = $(SOURCES:.cpp=.o)
TARGET = main
LIBS =

%.o: %.cpp
	$(CXX) -g -std=c++14 -o $@ -c $< -Wall

all: $(OBJECTS)
	$(CXX) -g -std=c++14 -o $(TARGET) $(OBJECTS) $(LIBS) -Wall

x: all
	./$(TARGET)

clean:
	rm -rf $(OBJECTS)

superclean: clean
	rm -rf $(TARGET)
