CXX = g++
CXXFLAGS = -Wall -std=c++17
TARGET = sim

SRCS = sim.cpp utils.cpp

all:
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
