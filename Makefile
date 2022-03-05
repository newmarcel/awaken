BUILD_DIR = build
DOCS_DIR = docs
DESTDIR = ~/install
BINARY_NAME = awaken

default: all

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) -r $(DOCS_DIR)

$(DOCS_DIR):
	doxygen Doxyfile

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

spm-clean:
	$(RM) -r ".build"
	$(RM) -r ".swiftpm"

spm-build:
	swift build

spm-run:
	swift run awaken

.PHONY: clean build release bin run install list-assertions log-assertions spm-clean spm-build spm-run
