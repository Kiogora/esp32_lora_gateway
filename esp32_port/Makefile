PROJECT_NAME := esp32_loragw

#Main folder compilational setter. Checks for variable BUILD-NORMAL. Otherwise, default build is cmdline at this development stage.
ifndef BUILD_NORMAL
EXTRA_COMPONENT_DIRS := $(abspath cmdline_tests)
else
EXTRA_COMPONENT_DIRS := $(abspath main)
endif

#Override component_dirs in project.mk
COMPONENT_DIRS := $(abspath components) $(EXTRA_COMPONENT_DIRS) $(IDF_PATH)/components $(IDF_PATH)/components/gsm

include $(IDF_PATH)/make/project.mk

