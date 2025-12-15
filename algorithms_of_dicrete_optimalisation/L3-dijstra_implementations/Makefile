CXX = g++
CXXFLAGS = -O3 -std=c++17 -Wall -Wextra
LOADLIBS = -lm

all: dijkstra dial radixheap

dijkstra: dijkstra.cpp
	$(CXX) $(CXXFLAGS) -o dijkstra dijkstra.cpp $(LOADLIBS)

dial: dijkstra.cpp
	$(CXX) $(CXXFLAGS) -o dial dijkstra.cpp $(LOADLIBS)

radixheap: dijkstra.cpp
	$(CXX) $(CXXFLAGS) -o radixheap dijkstra.cpp $(LOADLIBS)

clean:
	rm -f dijkstra dial radixheap *.o