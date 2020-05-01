CC := g++
ARGS := -ggdb -std=c++17 -lstdc++fs

BUILD := build
SRC := mosh

EXECUTABLE := mosh


all: $(BUILD)/$(EXECUTABLE)

run: clean all
	clear
	./$(BUILD)/$(EXECUTABLE)

$(BUILD)/$(EXECUTABLE) : $(SRC)/*
	$(CC) $(ARGS) $^ -o $@

clean:
	-rm $(BUILD)/*
