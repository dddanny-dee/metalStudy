CXX      = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS  = -framework Metal -framework Foundation -framework QuartzCore

TARGET = main
SRC    = main.cpp

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: run clean