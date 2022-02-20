CXX      := -c++
CXXFLAGS := -Werror
LDFLAGS  := -L/usr/lib -lstdc++ -lm -pthread -lwiringPi
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
TARGET   := lego_bot

INC_DIR = 	\
			$(application/inc/) \
			$(drivers/mc33926_motor_shield/inc/) \
			$(drivers/xbox360Controller/inc/) \
			$(schedulerTick_thread/inc/) \
			$(services/inc/) \

INC = $(foreach d, $(INC_DIR), -I$d)


SRC      :=                      \
   $(wildcard application/src/*.cpp) \
   $(wildcard drivers/mc33926_motor_shield/src/*.cpp) \
   $(wildcard drivers/xbox360Controller/src/*.cpp) \
   $(wildcard schedulerTick_thread/src/*.cpp) \

OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) -o $(APP_DIR)/$(TARGET) $(OBJECTS)

.PHONY: all build clean debug release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*