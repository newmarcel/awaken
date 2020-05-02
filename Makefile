BUILD_DIR = build
DESTDIR = ~/install
BINARY_NAME = awaken

default: all

clean:
	$(RM) -r $(BUILD_DIR)

build:
	meson $(BUILD_DIR)

release:
	meson $(BUILD_DIR) --buildtype=release

bin:
	ninja -C $(BUILD_DIR)

all: build bin

run:
	$(BUILD_DIR)/$(BINARY_NAME)

install:
	cd $(BUILD_DIR) && DESTDIR=$(DESTDIR) meson install

list-assertions:
	pmset -g assertions

log-assertions:
	pmset -g assertionslog

.PHONY: clean build release bin run install list-assertions log-assertions
