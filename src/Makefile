PROJECT = programmable-remote
BUILD_DIR = bin

CPPFILES = remote.cxx

DEVICE = stm32f103C8T6
OOCD_FILE = ./bluepill.cfg

CXXFLAGS = -DNDEBUG -O0 -g -fno-exceptions -fno-rtti -std=c++17 -Wno-shadow -Weffc++
LDFLAGS += -Wl,-Map=output.map


VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
INCLUDES += -I../sml/include
OPENCM3_DIR=../libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
