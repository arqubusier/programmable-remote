PROJECT = programmable-remote
BUILD_DIR = bin

CPPFILES = remote.cxx

DEVICE = stm32f103C8T6
OOCD_FILE = ./bluepill.cfg

CXXFLAGS = -fno-exceptions -fno-rtti

# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
OPENCM3_DIR=../libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
