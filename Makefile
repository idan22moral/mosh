CC := g++
ARGS := -ggdb -std=c++17 -lstdc++fs

BUILD := build
SRC := mosh

EXECUTABLE := mosh


all: $(BUILD)/$(EXECUTABLE)

run: clean all
	./$(BUILD)/$(EXECUTABLE)

$(BUILD)/$(EXECUTABLE) : $(SRC)/*
	[ ! -d $(BUILD) ] && mkdir build
	$(CC) $(ARGS) $^ -o $@

clean:
	-rm $(BUILD)/*
