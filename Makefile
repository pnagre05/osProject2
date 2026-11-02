CXX = g++
CXXFLAGS = -std=c++17 -pthread -g

all: my-dining

my-dining: my-dining.cpp
	$(CXX) $(CXXFLAGS) my-dining.cpp -o my-dining

clean:
	rm -f my-dining