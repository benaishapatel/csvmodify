BIN_NAME = CSVmodifier

SOURCES = CSVmodifier.cpp

CXX = g++
CXX_FLAGS = -std=c++11 -pedantic -Wall -Wextra -g -O2
LINK_FLAGS = -I/usr/local/include -lopencv_core -lopencv_imgproc -lopencv_highgui -L/usr/local/lib/

BUILD_PATH = build

OBJECTS = $(SOURCES:%.cpp=$(BUILD_PATH)/%.o)
DEP_FILES = $(OBJECTS:.o=.d)

.PHONY: all
all: $(BIN_NAME)

$(BUILD_PATH):
	@echo "Creating directory: $(BUILD_PATH)"
	@mkdir -p $(BUILD_PATH)

$(BUILD_PATH)/%.o: %.cpp | $(BUILD_PATH)
	@echo "Compiling: $< -> $@"
	@$(CXX) $(CXX_FLAGS) -MP -MMD -c -o $@ $<

$(BIN_NAME): $(OBJECTS)
	@echo "Linking: $(BIN_NAME)"
	@$(CXX) $(LINK_FLAGS) $(OBJECTS) -o $@

-include $(DEP_FILES)

.PHONY: clean
clean:
	@$(RM) -rv $(BIN_NAME) $(BUILD_PATH) *~ core *.core
