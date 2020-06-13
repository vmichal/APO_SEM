CC = arm-linux-gnueabihf-gcc
CXX = arm-linux-gnueabihf-g++

CPPFLAGS = -I .
CFLAGS =-g -std=gnu11 -Og -Wall
CXXFLAGS = -g -std=gnu++17 -Og -Wall
LDFLAGS = -lrt -lpthread

SOURCES = snake.cpp mzapo_phys.c mzapo_parlcd.c display.cpp led-line.cpp led-rgb.cpp font_prop14x16.c text.cpp knobs.cpp menu.cpp audio.cpp player.cpp game.cpp wArial_88.c help.cpp application.cpp map.cpp
#SOURCES += font_prop14x16.c font_rom8x16.c
TARGET_EXE = snake.elf
TARGET_IP ?= 192.168.0.102
ifeq ($(TARGET_IP)$(filter run,$(MAKECMDGOALS)),run)
$(warning The target IP address is not set)
$(warning Run as "TARGET_IP=192.168.202.xxx make run" or modify Makefile)
TARGET_IP ?= 192.168.202.xxx
endif
TARGET_DIR ?= /tmp/$(shell whoami)
TARGET_USER ?= root
# for use from Eduroam network use TARGET_IP=localhost and enable next line
#SSH_OPTIONS=-o "UserKnownHostsFile=/dev/null" -o "StrictHostKeyChecking=no" -o "Port=2222"
SSH_OPTIONS=-i ~/mzapo-root-key

OBJECTS += $(filter %.o,$(SOURCES:%.c=%.o))
OBJECTS += $(filter %.o,$(SOURCES:%.cpp=%.o))

#$(warning OBJECTS=$(OBJECTS))

ifeq ($(filter %.cpp,$(SOURCES)),)
LINKER = $(CC)
LDFLAGS += $(CFLAGS) $(CPPFLAGS)
else
LINKER = $(CXX)
LDFLAGS += $(CXXFLAGS) $(CPPFLAGS)
endif

%.o:%.c
	$(CC) $(CFLAGS) -c $<

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $<

all: $(TARGET_EXE)

$(TARGET_EXE): $(OBJECTS)
	$(LINKER) -L. $^ -o $@ $(LDFLAGS)

.PHONY : dep all run copy-executable debug

dep: depend

depend: $(SOURCES) *.h
	echo '# autogenerated dependencies' > depend
ifneq ($(filter %.c,$(SOURCES)),)
	$(CC) $(CFLAGS) $(CPPFLAGS) -w -E -M $(filter %.c,$(SOURCES)) \
	  >> depend
endif
ifneq ($(filter %.cpp,$(SOURCES)),)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -w -E -M $(filter %.cpp,$(SOURCES)) \
	  >> depend
endif

clean:
	rm -f *.o *.a $(OBJECTS) $(TARGET_EXE) connect.gdb depend

copy-executable: $(TARGET_EXE)
	ssh $(SSH_OPTIONS) -t $(TARGET_USER)@$(TARGET_IP) killall gdbserver 1>/dev/null 2>/dev/null || true
	ssh $(SSH_OPTIONS) $(TARGET_USER)@$(TARGET_IP) mkdir -p $(TARGET_DIR)
	scp $(SSH_OPTIONS) $(TARGET_EXE) $(TARGET_USER)@$(TARGET_IP):$(TARGET_DIR)/$(TARGET_EXE)

run: copy-executable $(TARGET_EXE)
	ssh $(SSH_OPTIONS) -t $(TARGET_USER)@$(TARGET_IP) $(TARGET_DIR)/$(TARGET_EXE)

debug: copy-executable $(TARGET_EXE)
	xterm -e ssh $(SSH_OPTIONS) -t $(TARGET_USER)@$(TARGET_IP) gdbserver :12345 $(TARGET_DIR)/$(TARGET_EXE) &
	sleep 2
	echo >connect.gdb "target extended-remote $(TARGET_IP):12345"
	echo >>connect.gdb "b main"
	echo >>connect.gdb "c"
	ddd --debugger gdb-multiarch -x connect.gdb $(TARGET_EXE)

-include depend
