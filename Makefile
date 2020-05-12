SOURCE_DIR := $(CURDIR)

# Do not print "Entering directory ..."
MAKEFLAGS += --no-print-directory

# The local configurations
-include local.mk

BUILD_DIR ?= $(SOURCE_DIR)/build
CMAKE_BUILD_TYPE ?= Debug
GENERATOR ?= 'Unix Makefiles'
CMAKE_PROG ?= $(shell (command -v cmake3 || echo cmake))

ifeq ($(CMAKE_ARGS),)
    CMAKE_ARGS := -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)
else
    CMAKE_ARGS += -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE)
endif

ifneq ($(CMAKE_INSTALL_PREFIX),)
    CMAKE_ARGS += -DCMAKE_INSTALL_PREFIX=$(CMAKE_INSTALL_PREFIX)
endif

ifneq ($(CMAKE_ARGS_EXTRA),)
    CMAKE_ARGS += $(CMAKE_ARGS_EXTRA)
endif

CMAKE_ARGS += $(SOURCE_DIR)

# Beautify Output
# Use 'make V=1' to see the full commands
#
# If IS_VERBOSE equals 0 then the commands will be hidden.
# If IS_VERBOSE equals 1 then the commands will be displayed.
IS_VERBOSE := 0
ifeq ("$(origin V)", "command line")
    IS_VERBOSE := $(V)
endif

ifneq ($(VERBOSE),)
    IS_VERBOSE := 1
endif

# Prefix commands with $(Q), that's useful for
# commands that shall be hidden in non-verbose mode.
#
#     $(Q)ln $@ :<
ifeq ($(IS_VERBOSE), 1)
    Q :=
else
    Q := @
endif

XMAKE := $(Q)$(MAKE)

PHONY += all
all: | $(BUILD_DIR)/.ran-cmake
	$(XMAKE) -C $(BUILD_DIR)

$(BUILD_DIR)/.ran-cmake:
	@if [ ! -d $(BUILD_DIR) ]; then mkdir -p $(BUILD_DIR); fi
	cd $(BUILD_DIR) && $(CMAKE_PROG) -G $(GENERATOR) $(CMAKE_ARGS)
	touch $@

PHONY += install
install: | all
	$(XMAKE) -C $(BUILD_DIR) install

PHONY += test
test: | all
	$(XMAKE) -C $(BUILD_DIR) test

PHONY += clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: $(PHONY)
