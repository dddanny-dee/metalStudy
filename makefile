CXX      = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS  = -framework Metal -framework Foundation -framework QuartzCore

TARGET = main
SRC    = main.cpp instance.cpp
BUILD  = build
OBJ    = $(SRC:%.cpp=$(BUILD)/%.o)
HDR    = instance.h

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(BUILD)/%.o: %.cpp $(HDR) | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(TARGET) $(BUILD)

.PHONY: run clean
