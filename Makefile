CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

SRC = main.cpp klv.cpp st0601.cpp st0102.cpp st0903.cpp stanag.cpp
OBJ = $(SRC:.cpp=.o)

main: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJ) main
